bt_status_t btsock_l2cap_listen(const char* name, int channel, int* sock_fd, int flags)
{
 return btsock_l2cap_listen_or_connect(name, NULL, channel, sock_fd, flags, 1);
}
