static BOOLEAN send_app_connect_signal(int fd, const bt_bdaddr_t* addr,
 int channel, int status, int send_fd, int tx_mtu)
{
 sock_connect_signal_t cs;
    cs.size = sizeof(cs);
    cs.bd_addr = *addr;
    cs.channel = channel;
    cs.status = status;
    cs.max_rx_packet_size = L2CAP_MAX_SDU_LENGTH;
    cs.max_tx_packet_size = tx_mtu;
 if (send_fd != -1) {
 if (sock_send_fd(fd, (const uint8_t*)&cs, sizeof(cs), send_fd) == sizeof(cs))
 return TRUE;
 else APPL_TRACE_ERROR("sock_send_fd failed, fd:%d, send_fd:%d", fd, send_fd);
 } else if (sock_send_all(fd, (const uint8_t*)&cs, sizeof(cs)) == sizeof(cs)) {
 return TRUE;
 }
 return FALSE;
}
