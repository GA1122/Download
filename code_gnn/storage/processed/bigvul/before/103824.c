void RenderView::OnPpapiBrokerChannelCreated(
    int request_id,
    base::ProcessHandle broker_process_handle,
    const IPC::ChannelHandle& handle) {
  pepper_delegate_.OnPpapiBrokerChannelCreated(request_id,
                                               broker_process_handle,
                                               handle);
}
