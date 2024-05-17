  virtual void OnClose(net::WebSocket* socket) {
    events_.push_back(
        WebSocketEvent(WebSocketEvent::EVENT_CLOSE, socket, std::string()));
    if (onclose_)
      onclose_->Run(&events_.back());
    if (callback_)
      callback_->Run(net::OK);
  }
