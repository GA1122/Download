  virtual void OnSyncProtocolError(
      const sessions::SyncSessionSnapshot& snapshot) {
    FailControllerInvocationIfDisabled("SyncProtocolError");
  }
