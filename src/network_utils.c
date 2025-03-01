/**
 * network_utils.c
 *
 * Implementation of network utility functions for TCP/IP packet manipulation.
 * Provides functionality for packet construction, checksums, and transmission.
 *
 * @author Mohamed Gebril
 * @date March 2025
 * @license MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/socket.h>
#include "headers/network_utils.h"

/**
 * Sends a raw IP packet to the specified destination.
 *
 * This function creates a raw socket, configures it appropriately,
 * and sends the provided IP packet to its destination address.
 *
 * @param packet_header Pointer to the IP header of the packet to send
 * @return void
 */
void network_send_raw_ip_packet(struct ipheader *packet_header)
{
    struct sockaddr_in destination_info;
    int socket_opt_enable = 1;
    int raw_socket;
    ssize_t bytes_sent;

    // Create a raw network socket
    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (raw_socket < 0)
    {
        fprintf(stderr, "[ERROR] Raw socket creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Set IP_HDRINCL option to indicate we're providing the IP header
    if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL,
                   &socket_opt_enable, sizeof(socket_opt_enable)) < 0)
    {
        fprintf(stderr, "[ERROR] Socket option setting failed: %s\n", strerror(errno));
        close(raw_socket);
        exit(EXIT_FAILURE);
    }

    // Configure destination information
    destination_info.sin_family = AF_INET;
    destination_info.sin_addr = packet_header->iph_destip;

    // Send the packet
    bytes_sent = sendto(raw_socket, packet_header, ntohs(packet_header->iph_len), 0,
                        (struct sockaddr *)&destination_info, sizeof(destination_info));

    if (bytes_sent < 0)
    {
        fprintf(stderr, "[ERROR] Packet transmission failed: %s\n", strerror(errno));
        close(raw_socket);
        exit(EXIT_FAILURE);
    }

    close(raw_socket);
}

/**
 * Calculates the Internet checksum for the given buffer.
 *
 * Implementation of the standard Internet checksum algorithm.
 * The checksum is calculated by summing up 16-bit words and
 * folding overflow bits back.
 *
 * @param buffer Pointer to the data buffer
 * @param length Length of the buffer in bytes
 * @return uint16_t The calculated checksum
 */
uint16_t network_calculate_checksum(uint16_t *buffer, int length)
{
    uint32_t checksum_sum = 0;
    uint16_t *word_ptr = buffer;
    int remaining_bytes = length;
    uint16_t odd_byte_temp = 0;

    // Sum up 16-bit words
    while (remaining_bytes > 1)
    {
        checksum_sum += *word_ptr++;
        remaining_bytes -= 2;
    }

    // Handle odd byte if present
    if (remaining_bytes == 1)
    {
        *(uint8_t *)(&odd_byte_temp) = *(uint8_t *)word_ptr;
        checksum_sum += odd_byte_temp;
    }

    // Fold 32-bit sum to 16 bits and compute one's complement
    checksum_sum = (checksum_sum >> 16) + (checksum_sum & 0xffff);
    checksum_sum += (checksum_sum >> 16);

    return (uint16_t)(~checksum_sum);
}

/**
 * Calculates TCP checksum using the pseudo header approach.
 *
 * Creates a pseudo header containing IP source/destination addresses,
 * protocol, and TCP length, then calculates the checksum over this
 * combined buffer.
 *
 * @param ip_header Pointer to the IP header containing the packet
 * @return uint16_t The calculated TCP checksum
 */
uint16_t network_calculate_tcp_checksum(struct ipheader *ip_header)
{
    struct tcpheader *tcp_header = (struct tcpheader *)((uint8_t *)ip_header +
                                                        sizeof(struct ipheader));
    int tcp_segment_length = ntohs(ip_header->iph_len) - sizeof(struct ipheader);

    // Create and initialize the pseudo TCP header for checksum calculation
    struct pseudo_tcp pseudo_header;
    memset(&pseudo_header, 0, sizeof(struct pseudo_tcp));

    // Fill in the pseudo header fields
    pseudo_header.saddr = ip_header->iph_sourceip.s_addr;
    pseudo_header.daddr = ip_header->iph_destip.s_addr;
    pseudo_header.mbz = 0;
    pseudo_header.ptcl = IPPROTO_TCP;
    pseudo_header.tcpl = htons(tcp_segment_length);

    // Copy the TCP header and data to the pseudo header
    memcpy(&pseudo_header.tcp, tcp_header, tcp_segment_length);

    // Calculate the checksum over the pseudo header
    return network_calculate_checksum((uint16_t *)&pseudo_header,
                                      tcp_segment_length + 12);
}

/**
 * Initializes a TCP header with appropriate values for a SYN packet.
 *
 * Sets up all fields in the TCP header including random source port
 * and sequence number. The checksum field is left at 0 for later calculation.
 *
 * @param tcp_header Pointer to the TCP header to initialize
 * @param dest_port Destination port number
 */
void network_initialize_tcp_header(struct tcpheader *tcp_header, uint16_t dest_port)
{
    tcp_header->tcp_sport = (uint16_t)rand(); // Random source port
    tcp_header->tcp_dport = htons(dest_port);
    tcp_header->tcp_seq = (uint32_t)rand(); // Random sequence number
    tcp_header->tcp_offx2 = 0x50;           // 5 words (20 bytes) - no options
    tcp_header->tcp_flags = TH_SYN;         // SYN flag for connection initiation
    tcp_header->tcp_win = htons(20000);     // Reasonable window size
    tcp_header->tcp_sum = 0;                // To be calculated later
    tcp_header->tcp_urp = 0;                // No urgent pointer
}

/**
 * Initializes an IP header with appropriate values.
 *
 * Sets up all fields in the IP header including a random source IP.
 * The header is configured for a TCP packet.
 *
 * @param ip_header Pointer to the IP header to initialize
 * @param dest_ip_str Destination IP address as a string
 */
void network_initialize_ip_header(struct ipheader *ip_header, const char *dest_ip_str)
{
    ip_header->iph_ver = 4;                  // IPv4
    ip_header->iph_ihl = 5;                  // 5 words (20 bytes) - no options
    ip_header->iph_tos = 0;                  // No special type of service
    ip_header->iph_ttl = 64;                 // Standard TTL value
    ip_header->iph_sourceip.s_addr = rand(); // Random source IP
    ip_header->iph_destip.s_addr = inet_addr(dest_ip_str);
    ip_header->iph_protocol = IPPROTO_TCP; // TCP protocol
    ip_header->iph_len = htons(sizeof(struct ipheader) +
                               sizeof(struct tcpheader));
    ip_header->iph_ident = htons((uint16_t)rand()); // Random ID
    ip_header->iph_offset = 0;                      // No fragmentation
    ip_header->iph_flag = 0;                        // No flags
    ip_header->iph_chksum = 0;                      // Not calculating IP checksum
                                                    // (kernel will do it)
}