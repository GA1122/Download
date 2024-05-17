void AppCacheDispatcherHost::OnStartUpdate(int host_id,
                                           IPC::Message* reply_msg) {
  if (pending_reply_msg_) {
    bad_message::ReceivedBadMessage(
        this, bad_message::ACDH_PENDING_REPLY_IN_START_UPDATE);
    delete reply_msg;
    return;
  }

  pending_reply_msg_.reset(reply_msg);
  if (appcache_service_.get()) {
    if (!backend_impl_.StartUpdateWithCallback(
            host_id, start_update_callback_, reply_msg)) {
      bad_message::ReceivedBadMessage(this, bad_message::ACDH_START_UPDATE);
    }
    return;
  }

  StartUpdateCallback(false, reply_msg);
}
