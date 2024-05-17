void RenderWidgetHostImpl::DidProcessFrame(uint32_t frame_token) {
  frame_token_message_queue_->DidProcessFrame(frame_token);
}
