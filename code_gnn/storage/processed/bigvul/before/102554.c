  virtual void RunWork() {
    bytes_written_ = base::WritePlatformFile(file_, offset_, buffer_.get(),
                                             bytes_to_write_);
    if (bytes_written_ < 0)
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
