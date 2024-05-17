static nl_sock * wifi_create_nl_socket(int port)
{
 struct nl_sock *sock = nl_socket_alloc();
 if (sock == NULL) {
        ALOGE("Could not create handle");
 return NULL;
 }

    wifi_socket_set_local_port(sock, port);

 struct sockaddr *addr = NULL;

 if (nl_connect(sock, NETLINK_GENERIC)) {
        ALOGE("Could not connect handle");
        nl_socket_free(sock);
 return NULL;
 }

  

 return sock;
}
