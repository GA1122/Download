void WebSocket::SkipReadBuffer(int len) {
  if (len == 0)
    return;
  DCHECK_GT(len, 0);
  read_consumed_len_ += len;
  int remaining = current_read_buf_->offset() - read_consumed_len_;
  DCHECK_GE(remaining, 0);
  if (remaining < read_consumed_len_ &&
      current_read_buf_->RemainingCapacity() < read_consumed_len_) {
    memmove(current_read_buf_->StartOfBuffer(),
            current_read_buf_->StartOfBuffer() + read_consumed_len_,
            remaining);
    read_consumed_len_ = 0;
    current_read_buf_->set_offset(remaining);
  }
}
