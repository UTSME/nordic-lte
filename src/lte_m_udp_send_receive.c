#include "lte_m_udp_send_receive.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lte_m_udp_send_receive);

void lte_m_udp_send(char *data, int length) {
	int err;

	LOG_INF("Transmitting UDP/IP payload of %d bytes", length + UDP_IP_HEADER_SIZE);

	err = send(lte_m_get_client_fd(), data, length, 0);
	if (err < 0) {
		LOG_ERR("Failed to transmit UDP packet, error: %d", err);
	}
}