bool BrowserRenderProcessHost::SendWithTimeout(IPC::Message* msg,
                                               int timeout_ms) {
  if (!channel_.get()) {
    delete msg;
    return false;
  }
  return channel_->SendWithTimeout(msg, timeout_ms);
}
