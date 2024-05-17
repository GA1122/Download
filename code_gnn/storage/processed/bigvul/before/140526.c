int HttpProxyClientSocket::RestartWithAuth(const CompletionCallback& callback) {
  DCHECK_EQ(STATE_NONE, next_state_);
  DCHECK(user_callback_.is_null());

  int rv = PrepareForAuthRestart();
  if (rv != OK)
    return rv;

  rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING) {
    if (!callback.is_null())
      user_callback_ =  callback;
  }

  return rv;
}
