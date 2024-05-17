void SocketStream::Finish(int result) {
  DCHECK(base::MessageLoop::current())
      << "The current base::MessageLoop must exist";
  DCHECK_EQ(base::MessageLoop::TYPE_IO, base::MessageLoop::current()->type())
      << "The current base::MessageLoop must be TYPE_IO";
  DCHECK_LE(result, OK);
  if (result == OK)
    result = ERR_CONNECTION_CLOSED;
  DCHECK_EQ(next_state_, STATE_NONE);
  DVLOG(1) << "Finish result=" << ErrorToString(result);

  metrics_->OnClose();

  if (result != ERR_CONNECTION_CLOSED && delegate_)
    delegate_->OnError(this, result);
  if (result != ERR_PROTOCOL_SWITCHED && delegate_)
    delegate_->OnClose(this);
  delegate_ = NULL;

  Release();
}
