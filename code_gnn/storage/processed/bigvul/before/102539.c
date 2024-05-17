  virtual void RunCallback() {
    callback_->Run(error_code(), created_);
    delete callback_;
  }
