void RenderWidgetHostImpl::OnFrameSwapMessagesReceived(
    uint32_t frame_token,
    std::vector<IPC::Message> messages) {
  frame_token_message_queue_->OnFrameSwapMessagesReceived(frame_token,
                                                          std::move(messages));
}
