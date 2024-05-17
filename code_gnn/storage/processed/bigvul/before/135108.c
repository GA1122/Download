void AppCacheDispatcherHost::OnRegisterHost(int host_id) {
  if (appcache_service_.get()) {
    if (!backend_impl_.RegisterHost(host_id)) {
      bad_message::ReceivedBadMessage(this, bad_message::ACDH_REGISTER);
    }
  }
}
