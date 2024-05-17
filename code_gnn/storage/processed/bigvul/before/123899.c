void RenderViewImpl::OnPpapiBrokerChannelCreated(
    int request_id,
    base::ProcessId broker_pid,
    const IPC::ChannelHandle& handle) {
  pepper_helper_->OnPpapiBrokerChannelCreated(request_id, broker_pid, handle);
}
