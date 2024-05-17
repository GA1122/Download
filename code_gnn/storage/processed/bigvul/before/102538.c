  virtual void RunCallback() {
    if (callback_) {
      callback_->Run(error_code());
      delete callback_;
    }
  }
