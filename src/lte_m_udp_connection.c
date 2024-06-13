#include "lte_m_udp_connection.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lte_m_udp_connection);

static int client_fd;
static struct sockaddr_storage host_addr;

K_SEM_DEFINE(lte_connected_sem, 0, 1);

static void lte_m_handler(const struct lte_lc_evt *const evt) {
	switch (evt->type) {
		case LTE_LC_EVT_NW_REG_STATUS:
			if ((evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_HOME) &&
				(evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_ROAMING)) {
				break;
			}

			LOG_INF("Network registration status: %s",
				evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_HOME ?
				"Connected - home" : "Connected - roaming");
			
			//Release semaphore as the lte network is now connected to
			k_sem_give(&lte_connected_sem);
			break;

			case LTE_LC_EVT_PSM_UPDATE:
				LOG_INF("PSM parameter update: TAU: %d s, Active time: %d s",
					evt->psm_cfg.tau, evt->psm_cfg.active_time);
				break;

			case LTE_LC_EVT_EDRX_UPDATE:
				LOG_INF("eDRX parameter update: eDRX: %.2f s, PTW: %.2f s",
					(double)evt->edrx_cfg.edrx, (double)evt->edrx_cfg.ptw);
				break;

			case LTE_LC_EVT_RRC_UPDATE:
				LOG_INF("RRC mode: %s",
					evt->rrc_mode == LTE_LC_RRC_MODE_CONNECTED ? "Connected" : "Idle");
				break;

			case LTE_LC_EVT_CELL_UPDATE:
				LOG_INF("LTE cell changed: Cell ID: %d, Tracking area: %d",
					evt->cell.id, evt->cell.tac);
				break;

			default:
				break;
	}
}

//Connects to server
static int lte_m_udp_socket_connect() {
	int err;

	client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_fd < 0) {
		LOG_ERR("Failed to create UDP socket, error: %d", errno);
		err = -errno;
		return err;
	}

	err = connect(client_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr_in));
	if (err < 0) {
		LOG_ERR("Failed to connect socket, error: %d", errno);
		close(client_fd);
		return err;
	}

	return 0;
}

int lte_m_get_client_fd() {
	return client_fd;
}

int lte_m_udp_connect(char *server_ip, int server_port) {
	int err;

	//Connects to lte network
	err = lte_lc_connect_async(lte_m_handler);
	if (err) {
		LOG_ERR("Failed to connect to LTE network, error: %d", err);
		return -1;
	}

	//Wait forever until connected to the lte network before setting up the socket and connecting to it
	k_sem_take(&lte_connected_sem, K_FOREVER);
	
	//Setup the socket
	struct sockaddr_in *server = ((struct sockaddr_in *)&host_addr);

	server->sin_family = AF_INET;
	server->sin_port = htons(server_port);

	inet_pton(AF_INET, server_ip, &server->sin_addr);
	
	//Connect to the socket
	LOG_INF("Conecting to: %s:%d", server_ip, server_port);

	err = lte_m_udp_socket_connect();
	if (err) {
		return -1;
	}

	//No errors
	return 0;
}
