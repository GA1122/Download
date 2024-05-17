  WebSocketEvent(EventType type, net::WebSocket* websocket,
                 const std::string& websocket_msg)
      : event_type(type), socket(websocket), msg(websocket_msg) {}
