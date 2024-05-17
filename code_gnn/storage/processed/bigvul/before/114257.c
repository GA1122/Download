void GpuChannelHost::OnChannelError() {
  state_ = kLost;

  channel_.reset();
}
