int HttpProxyClientSocket::Connect(const CompletionCallback& callback) {
  DCHECK(transport_.get());
  DCHECK(transport_->socket());
  DCHECK(user_callback_.is_null());

  if (using_spdy_ || !tunnel_)
    next_state_ = STATE_DONE;
  if (next_state_ == STATE_DONE)
    return OK;

  DCHECK_EQ(STATE_NONE, next_state_);
  next_state_ = STATE_GENERATE_AUTH_TOKEN;

  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;
  return rv;
}
