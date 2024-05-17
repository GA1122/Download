  virtual void SetUp() {
    mock_socket_factory_.reset();
    handshake_request_ = kWebSocketHandshakeRequest;
    handshake_response_ = kWebSocketHandshakeResponse;
  }
