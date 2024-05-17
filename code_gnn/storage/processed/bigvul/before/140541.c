void SpdyProxyClientSocket::Disconnect() {
  read_buffer_queue_.Clear();
  user_buffer_ = NULL;
  user_buffer_len_ = 0;
  read_callback_.Reset();

  write_buffer_len_ = 0;
  write_callback_.Reset();
  write_callback_weak_factory_.InvalidateWeakPtrs();

  next_state_ = STATE_DISCONNECTED;

  if (spdy_stream_.get()) {
    spdy_stream_->Cancel();
    DCHECK(!spdy_stream_.get());
  }
}
