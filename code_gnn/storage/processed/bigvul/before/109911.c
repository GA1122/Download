void GpuProcessHost::OnChannelConnected(int32 peer_pid) {
  TRACE_EVENT0("gpu", "GpuProcessHostUIShim::OnChannelConnected");

  while (!queued_messages_.empty()) {
    Send(queued_messages_.front());
    queued_messages_.pop();
  }
}
