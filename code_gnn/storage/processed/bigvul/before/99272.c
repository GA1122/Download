void ResourceMessageFilter::OnChannelClosing() {
  channel_ = NULL;

  resource_dispatcher_host_->CancelRequestsForProcess(id());

  audio_renderer_host_->IPCChannelClosing();
}
