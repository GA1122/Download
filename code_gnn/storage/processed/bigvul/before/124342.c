void MessageService::FinalizeOpenChannelToNativeApp(
    int receiver_port_id,
    const std::string& channel_name,
    scoped_ptr<MessageChannel> channel,
    const std::string& tab_json,
    NativeMessageProcessHost::ScopedHost native_process) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));

  if (!native_process.get()) {
    LOG(ERROR) << "Failed to create native process.";
    return;
  }
  channel->receiver.reset(new NativeMessagePort(native_process.release()));

  channel->opener->IncrementLazyKeepaliveCount();

  AddChannel(channel.release(), receiver_port_id);
}
