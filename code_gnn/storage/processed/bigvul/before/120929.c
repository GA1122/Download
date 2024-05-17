int SocketStream::DoBeforeConnectComplete(int result) {
  DCHECK_NE(ERR_IO_PENDING, result);

  if (result == OK)
    next_state_ = STATE_RESOLVE_PROXY;
  else
    next_state_ = STATE_CLOSE;

  return result;
}
