int SocketStream::DoResolveHostComplete(int result) {
  if (result == OK)
    next_state_ = STATE_RESOLVE_PROTOCOL;
  else
    next_state_ = STATE_CLOSE;
  return result;
}
