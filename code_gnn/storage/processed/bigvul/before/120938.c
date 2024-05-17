int SocketStream::DoResolveProtocol(int result) {
  DCHECK_EQ(OK, result);

  if (!delegate_) {
    next_state_ = STATE_CLOSE;
    return result;
  }

  next_state_ = STATE_RESOLVE_PROTOCOL_COMPLETE;
  result = delegate_->OnStartOpenConnection(this, io_callback_);
  if (result == ERR_IO_PENDING)
    metrics_->OnWaitConnection();
  else if (result != OK && result != ERR_PROTOCOL_SWITCHED)
    next_state_ = STATE_CLOSE;
  return result;
}
