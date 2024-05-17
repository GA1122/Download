  virtual ~WebSocketEventRecorder() {
    delete onopen_;
    delete onmessage_;
    delete onclose_;
  }
