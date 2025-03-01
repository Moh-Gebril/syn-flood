/**
 * ip_header.h
 *
 * Defines the IP header structure and related constants for
 * network packet manipulation.
 *
 * @author Mohamed Gebril
 * @date March 2025
 * @license MIT
 */

#ifndef IP_HEADER_H
#define IP_HEADER_H

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdint.h>

/**
 * IPv4 header structure.
 *
 * This structure represents the IPv4 header as defined in RFC 791.
 * It uses bit fields to map to the exact header format.
 */
struct ipheader
{
    uint8_t iph_ihl : 4,         /**< IP header length (in 32-bit words) */
        iph_ver : 4;             /**< IP version (4 for IPv4) */
    uint8_t iph_tos;             /**< Type of service */
    uint16_t iph_len;            /**< Total packet length (in bytes) */
    uint16_t iph_ident;          /**< Identification for fragmentation */
    uint16_t iph_flag : 3,       /**< Fragmentation flags */
        iph_offset : 13;         /**< Fragment offset (in 8-byte units) */
    uint8_t iph_ttl;             /**< Time to live */
    uint8_t iph_protocol;        /**< Protocol (TCP, UDP, etc.) */
    uint16_t iph_chksum;         /**< Header checksum */
    struct in_addr iph_sourceip; /**< Source IP address */
    struct in_addr iph_destip;   /**< Destination IP address */
};

#endif // IP_HEADER_H
