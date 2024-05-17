int SpdyProxyClientSocket::Connect(const CompletionCallback& callback) {
  DCHECK(read_callback_.is_null());
  if (next_state_ == STATE_OPEN)
    return OK;

  DCHECK_EQ(STATE_DISCONNECTED, next_state_);
  next_state_ = STATE_GENERATE_AUTH_TOKEN;

  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    read_callback_ = callback;
  return rv;
}
