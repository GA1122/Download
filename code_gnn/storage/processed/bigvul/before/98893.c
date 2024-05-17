  void InitReadBuf(WebSocket* websocket) {
    websocket->current_read_buf_ = new GrowableIOBuffer();
  }
