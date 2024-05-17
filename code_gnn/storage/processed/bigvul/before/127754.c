int MockNetworkTransaction::Read(net::IOBuffer* buf,
                                 int buf_len,
                                 const CompletionCallback& callback) {
  CHECK(!done_reading_called_);
  int num = 0;
  if (read_handler_) {
    num = (*read_handler_)(content_length_, data_cursor_, buf, buf_len);
    data_cursor_ += num;
  } else {
    int data_len = static_cast<int>(data_.size());
    num = std::min(static_cast<int64_t>(buf_len), data_len - data_cursor_);
    if (test_mode_ & TEST_MODE_SLOW_READ)
      num = std::min(num, 1);
    if (num) {
      memcpy(buf->data(), data_.data() + data_cursor_, num);
      data_cursor_ += num;
    }
  }
  if (test_mode_ & TEST_MODE_SYNC_NET_READ)
    return num;

  CallbackLater(callback, num);
  return ERR_IO_PENDING;
}
