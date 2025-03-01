/**
 * network_utils.h
 *
 * Defines utility functions and structures for network packet
 * manipulation and transmission.
 *
 * @author Mohamed Gebril
 * @date March 2025
 * @license MIT
 */

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>
#include "ip_header.h"
#include "tcp_header.h"

/** Maximum packet length in bytes */
#define PACKET_LEN 1500

/**
 * Pseudo TCP header structure.
 *
 * Used for TCP checksum calculation as defined in RFC 793.
 * This structure combines IP addressing information with TCP length
 * to create the "pseudo header" used in the checksum algorithm.
 */
struct pseudo_tcp
{
    uint32_t saddr;           /**< Source IP address */
    uint32_t daddr;           /**< Destination IP address */
    uint8_t mbz;              /**< Reserved, must be zero */
    uint8_t ptcl;             /**< Protocol (IPPROTO_TCP) */
    uint16_t tcpl;            /**< TCP segment length */
    struct tcpheader tcp;     /**< TCP header */
    char payload[PACKET_LEN]; /**< TCP payload */
};

/**
 * Sends a raw IP packet to the specified destination.
 *
 * @param packet_header Pointer to the IP header of the packet to send
 */
void network_send_raw_ip_packet(struct ipheader *packet_header);

/**
 * Calculates the Internet checksum for the given buffer.
 *
 * @param buffer Pointer to the data buffer
 * @param length Length of the buffer in bytes
 * @return uint16_t The calculated checksum
 */
uint16_t network_calculate_checksum(uint16_t *buffer, int length);

/**
 * Calculates TCP checksum using the pseudo header approach.
 *
 * @param ip_header Pointer to the IP header containing the packet
 * @return uint16_t The calculated TCP checksum
 */
uint16_t network_calculate_tcp_checksum(struct ipheader *ip_header);

/**
 * Initializes a TCP header with appropriate values for a SYN packet.
 *
 * @param tcp_header Pointer to the TCP header to initialize
 * @param dest_port Destination port number
 */
void network_initialize_tcp_header(struct tcpheader *tcp_header, uint16_t dest_port);

/**
 * Initializes an IP header with appropriate values.
 *
 * @param ip_header Pointer to the IP header to initialize
 * @param dest_ip_str Destination IP address as a string
 */
void network_initialize_ip_header(struct ipheader *ip_header, const char *dest_ip_str);

#endif // NETWORK_UTILS_H