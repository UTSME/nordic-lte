cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(lte_m)

target_sources(app PRIVATE src/main.c)

zephyr_include_directories(src)

target_sources(app PRIVATE src/lte_m_udp_connection.c)
target_sources(app PRIVATE src/lte_m_udp_send_receive.c)
