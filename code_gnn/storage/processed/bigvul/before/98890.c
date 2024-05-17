  void AddToReadBuf(WebSocket* websocket, const char* data, int len) {
    websocket->AddToReadBuffer(data, len);
  }
