void WebSocketJob::Close() {
  if (state_ == CLOSED)
    return;

  state_ = CLOSING;
  if (current_buffer_) {
    return;
  }
  state_ = CLOSED;
  CloseInternal();
}
