  virtual void RunCallback() {
    if (callback_) {
      callback_->Run(error_code(), buffer_.get(), bytes_read_);
      delete callback_;
    }
  }
