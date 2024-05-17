  virtual void DoSendWebSocketHandshake(SocketStreamEvent* event) {
    event->socket->SendData(
        handshake_request_.data(), handshake_request_.size());
  }
