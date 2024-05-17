void WebSocketJob::SendPending() {
  if (current_buffer_)
    return;
  if (send_frame_handler_->UpdateCurrentBuffer(false) <= 0) {
    if (state_ == CLOSING)
      CloseInternal();
    return;
  }
  current_buffer_ = new DrainableIOBuffer(
      send_frame_handler_->GetCurrentBuffer(),
      send_frame_handler_->GetCurrentBufferSize());
  SendDataInternal(current_buffer_->data(), current_buffer_->BytesRemaining());
}
