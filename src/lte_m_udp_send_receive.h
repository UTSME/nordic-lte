#ifndef LTE_M_UDP_SEND_RECEIVE_H
#define LTE_M_UDP_SEND_RECEIVE_H

#include <zephyr/net/socket.h>

#include "lte_m_udp_connection.h"

#define UDP_IP_HEADER_SIZE 28

// Sends data to the socket
void lte_m_udp_send(char *data, int length);

#endif