bool GpuChannelHost::MessageFilter::OnMessageReceived(
    const IPC::Message& message) {
  DCHECK(parent_->factory_->IsIOThread());
  if (message.is_reply())
    return false;

  DCHECK(message.routing_id() != MSG_ROUTING_CONTROL);

  ListenerMap::iterator it = listeners_.find(message.routing_id());

  if (it != listeners_.end()) {
    const GpuListenerInfo& info = it->second;
    info.loop->PostTask(
        FROM_HERE,
        base::Bind(
            base::IgnoreResult(&IPC::Channel::Listener::OnMessageReceived),
            info.listener,
            message));
  }

  return true;
}
