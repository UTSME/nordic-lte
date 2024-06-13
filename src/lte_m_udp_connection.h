#ifndef LTE_M_UDP_CONNECTION_H
#define LTE_M_UDP_CONNECTION_H

#include <zephyr/net/socket.h>
#include <modem/lte_lc.h>
#include <modem/nrf_modem_lib.h>
#include <nrf_modem_at.h>

//Should only be used in the lte_m_udp_send_receive.c file
int lte_m_get_client_fd();

// Connects to lte-m network, sets up socket and then calls lte_m_udp_socket_connect to connect to the socket
int lte_m_udp_connect(char *server_ip, int server_port);

// Connects to the socket specified in lte_m_udp_connect function after lte_m_udp_connect sets up the socket
// static int lte_m_udp_socket_connect()

// Handler function for events relating to the lte-m modem
// static void lte_m_handler(const struct lte_lc_evt *const evt)

#endif