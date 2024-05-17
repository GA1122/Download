static bool send_app_connect_signal(int fd, const bt_bdaddr_t* addr, int channel, int status, int send_fd) {
 sock_connect_signal_t cs;
  cs.size = sizeof(cs);
  cs.bd_addr = *addr;
  cs.channel = channel;
  cs.status = status;
  cs.max_rx_packet_size = 0;  
  cs.max_tx_packet_size = 0;  
 if (send_fd == INVALID_FD)
 return sock_send_all(fd, (const uint8_t *)&cs, sizeof(cs)) == sizeof(cs);

 return sock_send_fd(fd, (const uint8_t *)&cs, sizeof(cs), send_fd) == sizeof(cs);
}
