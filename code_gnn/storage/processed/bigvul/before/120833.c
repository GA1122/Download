  virtual void TearDown() {
    adapter_ = NULL;
    DBusThreadManager::Shutdown();
  }
