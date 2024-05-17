void AppCacheDispatcherHost::OnSwapCache(int host_id, IPC::Message* reply_msg) {
  if (pending_reply_msg_) {
    bad_message::ReceivedBadMessage(
        this, bad_message::ACDH_PENDING_REPLY_IN_SWAP_CACHE);
    delete reply_msg;
    return;
  }

  pending_reply_msg_.reset(reply_msg);
  if (appcache_service_.get()) {
    if (!backend_impl_.SwapCacheWithCallback(
            host_id, swap_cache_callback_, reply_msg)) {
      bad_message::ReceivedBadMessage(this, bad_message::ACDH_SWAP_CACHE);
    }
    return;
  }

  SwapCacheCallback(false, reply_msg);
}
