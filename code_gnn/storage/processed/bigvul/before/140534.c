int HttpProxyClientSocket::Write(IOBuffer* buf, int buf_len,
                                 const CompletionCallback& callback) {
  DCHECK_EQ(STATE_DONE, next_state_);
  DCHECK(user_callback_.is_null());

  return transport_->socket()->Write(buf, buf_len, callback);
}
