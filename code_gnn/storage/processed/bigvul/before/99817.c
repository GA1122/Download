bool WebPluginDelegateStub::Send(IPC::Message* msg) {
  return channel_->Send(msg);
}
