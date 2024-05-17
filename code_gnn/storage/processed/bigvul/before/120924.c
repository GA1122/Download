void SocketStream::DetachDelegate() {
  if (!delegate_)
    return;
  delegate_ = NULL;
  if (next_state_ == STATE_NONE)
    return;
  net_log_.AddEvent(NetLog::TYPE_CANCELLED);
  pending_write_bufs_.clear();
  Close();
}
