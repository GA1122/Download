  int OnRead(IOBuffer* buf,
             int buf_length,
             const CompletionCallback& callback) {
    if (read_result_ > 0)
      bytes_remaining_ = std::max(0, bytes_remaining_ - read_result_);
    if (IsInMemory()) {
      return read_result_;
    } else {
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::Bind(callback, read_result_));
      return ERR_IO_PENDING;
    }
  }
