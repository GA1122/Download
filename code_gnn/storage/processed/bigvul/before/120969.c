bool SocketStream::SendData(const char* data, int len) {
  DCHECK(base::MessageLoop::current())
      << "The current base::MessageLoop must exist";
  DCHECK_EQ(base::MessageLoop::TYPE_IO, base::MessageLoop::current()->type())
      << "The current base::MessageLoop must be TYPE_IO";
  DCHECK_GT(len, 0);

  if (!socket_.get() || !socket_->IsConnected() || next_state_ == STATE_NONE)
    return false;

  int total_buffered_bytes = len;
  if (current_write_buf_.get()) {
    total_buffered_bytes += current_write_buf_->size();
  }
  total_buffered_bytes += GetTotalSizeOfPendingWriteBufs();
  if (total_buffered_bytes > max_pending_send_allowed_)
    return false;

  pending_write_bufs_.push_back(make_scoped_refptr(
      new IOBufferWithSize(len)));
  memcpy(pending_write_bufs_.back()->data(), data, len);

  if (!current_write_buf_.get()) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE, base::Bind(&SocketStream::DoLoop, this, OK));
  }

  return true;
}
