void btsnoop_net_close() {
#if (!defined(BT_NET_DEBUG) || (BT_NET_DEBUG != TRUE))
 return;  
#endif

 if (listen_thread_valid_) {
    shutdown(listen_socket_, SHUT_RDWR);
    pthread_join(listen_thread_, NULL);
    safe_close_(&client_socket_);
    listen_thread_valid_ = false;
 }
}
