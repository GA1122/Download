  void DoClose(WebSocketEvent* event) {
    event->socket->Close();
  }
