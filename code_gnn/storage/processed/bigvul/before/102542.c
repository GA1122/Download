  virtual void RunCallback() {
    if (callback_) {
      callback_->Run(error_code(), bytes_written_);
      delete callback_;
    }
  }
