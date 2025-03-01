/**
 * tcp_header.h
 *
 * Defines the TCP header structure and related constants for
 * network packet manipulation.
 *
 * @author Mohamed Gebril
 * @date March 2025
 * @license MIT
 */

#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include <stdint.h>

/**
 * TCP header structure.
 *
 * This structure represents the TCP header as defined in RFC 793.
 * It contains all fields needed for a valid TCP segment.
 */
struct tcpheader
{
    uint16_t tcp_sport; /**< Source port */
    uint16_t tcp_dport; /**< Destination port */
    uint32_t tcp_seq;   /**< Sequence number */
    uint32_t tcp_ack;   /**< Acknowledgment number */
    uint8_t tcp_offx2;  /**< Data offset (4 bits) and reserved (4 bits) */
    uint8_t tcp_flags;  /**< TCP flags (FIN, SYN, RST, etc.) */
    uint16_t tcp_win;   /**< Window size */
    uint16_t tcp_sum;   /**< Checksum */
    uint16_t tcp_urp;   /**< Urgent pointer */
};

/** TCP Flag definitions */
#define TH_FIN 0x01  /**< End of data flag */
#define TH_SYN 0x02  /**< Synchronize sequence numbers flag */
#define TH_RST 0x04  /**< Reset connection flag */
#define TH_PUSH 0x08 /**< Push function flag */
#define TH_ACK 0x10  /**< Acknowledgment field significant flag */
#define TH_URG 0x20  /**< Urgent pointer field significant flag */
#define TH_ECE 0x40  /**< ECN-Echo flag */
#define TH_CWR 0x80  /**< Congestion Window Reduced flag */

/** All flags combined for masking operations */
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)

/** Macro to extract data offset from tcp_offx2 field */
#define TH_OFF(th) (((th)->tcp_offx2 & 0xf0) >> 4)

#endif // TCP_HEADER_H
