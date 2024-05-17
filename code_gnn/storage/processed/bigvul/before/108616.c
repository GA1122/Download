  void DoCallback(const CompletionCallback& callback) {
    callback.Run(callback_retval_);
  }
