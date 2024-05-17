  virtual void TearDown() {
    mock_server_.reset();
    delete syncer_;
    syncer_ = NULL;
    syncdb_.TearDown();
  }
