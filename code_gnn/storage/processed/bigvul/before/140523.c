void HttpProxyClientSocket::OnIOComplete(int result) {
  DCHECK_NE(STATE_NONE, next_state_);
  DCHECK_NE(STATE_DONE, next_state_);
  int rv = DoLoop(result);
  if (rv != ERR_IO_PENDING)
    DoCallback(rv);
}
