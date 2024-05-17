int SocketStream::DidReceiveData(int result) {
  DCHECK(read_buf_.get());
  DCHECK_GT(result, 0);
  net_log_.AddEvent(NetLog::TYPE_SOCKET_STREAM_RECEIVED);
  int len = result;
  metrics_->OnRead(len);
  if (delegate_) {
    delegate_->OnReceivedData(this, read_buf_->data(), len);
  }
  read_buf_ = NULL;
  return OK;
}
