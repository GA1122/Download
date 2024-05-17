void SocketStream::DidSendData(int result) {
  DCHECK_GT(result, 0);
  DCHECK(current_write_buf_.get());
  net_log_.AddEvent(NetLog::TYPE_SOCKET_STREAM_SENT);

  int bytes_sent = result;

  metrics_->OnWrite(bytes_sent);

  current_write_buf_->DidConsume(result);

  if (current_write_buf_->BytesRemaining())
    return;

  size_t bytes_freed = current_write_buf_->size();

  current_write_buf_ = NULL;

  if (delegate_)
    delegate_->OnSentData(this, bytes_freed);
}
