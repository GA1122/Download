  virtual void RunCallback() {
    callback_->Run(error_code(), file_info_);
    delete callback_;
  }
