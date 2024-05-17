void RenderViewImpl::OnPpapiBrokerChannelCreated(
    int request_id,
    const IPC::ChannelHandle& handle) {
  pepper_delegate_.OnPpapiBrokerChannelCreated(request_id,
                                               handle);
}
