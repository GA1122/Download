void btsnoop_net_open() {
#if (!defined(BT_NET_DEBUG) || (BT_NET_DEBUG != TRUE))
 return;  
#endif

  listen_thread_valid_ = (pthread_create(&listen_thread_, NULL, listen_fn_, NULL) == 0);
 if (!listen_thread_valid_) {
    LOG_ERROR("%s pthread_create failed: %s", __func__, strerror(errno));
 } else {
    LOG_DEBUG("initialized");
 }
}
