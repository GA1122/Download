void SpdyProxyClientSocket::OnClose(int status)  {
  was_ever_used_ = spdy_stream_->WasEverUsed();
  spdy_stream_.reset();

  bool connecting = next_state_ != STATE_DISCONNECTED &&
      next_state_ < STATE_OPEN;
  if (next_state_ == STATE_OPEN)
    next_state_ = STATE_CLOSED;
  else
    next_state_ = STATE_DISCONNECTED;

  base::WeakPtr<SpdyProxyClientSocket> weak_ptr = weak_factory_.GetWeakPtr();
  CompletionCallback write_callback = write_callback_;
  write_callback_.Reset();
  write_buffer_len_ = 0;

  if (connecting) {
    DCHECK(!read_callback_.is_null());
    CompletionCallback read_callback = read_callback_;
    read_callback_.Reset();
    read_callback.Run(status);
  } else if (!read_callback_.is_null()) {
    OnDataReceived(scoped_ptr<SpdyBuffer>());
  }
  if (weak_ptr.get() && !write_callback.is_null())
    write_callback.Run(ERR_CONNECTION_CLOSED);
}
