#include <stdio.h>
#include <zephyr/kernel.h>

#include "lte_m_udp_main.h"

K_SEM_DEFINE(modem_shutdown_sem, 0, 1);

int main(void) {
	int err;

	printk("UDP sample has started\n");

	err = nrf_modem_lib_init();
	if (err) {
		printk("Failed to initialize modem library, error: %d\n", err);
		return -1;
	}

	err = lte_m_udp_connect(CONFIG_UDP_SERVER_ADDRESS_STATIC, CONFIG_UDP_SERVER_PORT);
	if (err) {
		return -1;
	}

	char data[] = "this is some test data";
	lte_m_udp_send(data, sizeof(data));

	/*
	this does nothing rn will need to change it so the semaphore is taken when modem is in use and then released when it is done
	so it can be taken here and then the modem can be shutdown
	*/
	k_sem_take(&modem_shutdown_sem, K_FOREVER);

	err = nrf_modem_lib_shutdown();
	if (err) {
		return -1;
	}

	return 0;
}
