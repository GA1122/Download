  virtual void OnOpen(net::WebSocket* socket) {
    events_.push_back(
        WebSocketEvent(WebSocketEvent::EVENT_OPEN, socket, std::string()));
    if (onopen_)
      onopen_->Run(&events_.back());
  }
