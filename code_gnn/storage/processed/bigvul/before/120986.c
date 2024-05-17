  virtual MockClientSocketFactory* GetMockClientSocketFactory() {
    mock_socket_factory_.reset(new MockClientSocketFactory);
    return mock_socket_factory_.get();
  }
