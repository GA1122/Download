  virtual void TearDown() {
    if (invalidation_notifier_.get())
      ResetNotifier();
  }
