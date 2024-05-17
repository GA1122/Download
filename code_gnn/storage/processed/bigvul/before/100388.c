void BrowserRenderProcessHost::ReceivedBadMessage(uint16 msg_type) {
  BadMessageTerminateProcess(msg_type, process_.handle());
}
