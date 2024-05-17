bool PluginChannel::Send(IPC::Message* msg) {
  in_send_++;
  if (log_messages_) {
    VLOG(1) << "sending message @" << msg << " on channel @" << this
            << " with type " << msg->type();
  }
  bool result = NPChannelBase::Send(msg);
  in_send_--;
  return result;
}
