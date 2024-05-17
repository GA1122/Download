  virtual void TearDown() {
    mock_server_.reset();
    delete syncer_;
    syncer_ = NULL;
    dir_maker_.TearDown();
  }
