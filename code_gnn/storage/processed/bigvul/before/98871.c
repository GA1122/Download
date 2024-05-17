void WebSocket::Close() {
  DCHECK(MessageLoop::current() == origin_loop_);

  if (ready_state_ == INITIALIZED) {
    DCHECK(!socket_stream_);
    ready_state_ = CLOSED;
    return;
  }
  if (ready_state_ != CLOSED) {
    DCHECK(socket_stream_);
    socket_stream_->Close();
    return;
  }
}
