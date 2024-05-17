  virtual void DidFail(FileError::ErrorCode) {
    callback_->sendFailure(Response::Error("Couldn't read BLOB"));
    Dispose();
  }
