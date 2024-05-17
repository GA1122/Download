static inline BOOLEAN send_app_psm_or_chan_l(l2cap_socket *sock)
{
 return sock_send_all(sock->our_fd, (const uint8_t*)&sock->channel, sizeof(sock->channel))
 == sizeof(sock->channel);
}
