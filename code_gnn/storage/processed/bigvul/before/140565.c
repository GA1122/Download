int SpdyProxyClientSocket::Read(IOBuffer* buf, int buf_len,
                                const CompletionCallback& callback) {
  DCHECK(read_callback_.is_null());
  DCHECK(!user_buffer_.get());

  if (next_state_ == STATE_DISCONNECTED)
    return ERR_SOCKET_NOT_CONNECTED;

  if (next_state_ == STATE_CLOSED && read_buffer_queue_.IsEmpty()) {
    return 0;
  }

  DCHECK(next_state_ == STATE_OPEN || next_state_ == STATE_CLOSED);
  DCHECK(buf);
  size_t result = PopulateUserReadBuffer(buf->data(), buf_len);
  if (result == 0) {
    user_buffer_ = buf;
    user_buffer_len_ = static_cast<size_t>(buf_len);
    DCHECK(!callback.is_null());
    read_callback_ = callback;
    return ERR_IO_PENDING;
  }
  user_buffer_ = NULL;
  return result;
}
