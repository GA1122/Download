  virtual void RunWork() {
    bytes_read_ = base::ReadPlatformFile(file_, offset_, buffer_.get(),
                                         bytes_to_read_);
    if (bytes_read_ < 0)
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
