  virtual void RunCallback() {
    callback_->Run(error_code(), base::PassPlatformFile(&file_handle_),
                   created_);
    delete callback_;
  }
