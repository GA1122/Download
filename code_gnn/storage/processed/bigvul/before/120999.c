  virtual void SetWebSocketHandshakeMessage(
      const char* request, const char* response) {
    handshake_request_ = request;
    handshake_response_ = response;
  }
