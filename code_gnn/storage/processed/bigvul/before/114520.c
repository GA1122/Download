bool PluginChannel::OnMessageReceived(const IPC::Message& msg) {
  if (log_messages_) {
    VLOG(1) << "received message @" << &msg << " on channel @" << this
            << " with type " << msg.type();
  }
  return NPChannelBase::OnMessageReceived(msg);
}
