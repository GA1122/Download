bool WebSocketHandshake::is_secure() const {
   return url_.SchemeIs("wss");
 }
