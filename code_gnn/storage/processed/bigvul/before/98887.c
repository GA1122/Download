 WebSocketHandshake::WebSocketHandshake(
     const GURL& url,
     const std::string& origin,
    const std::string& location,
    const std::string& protocol)
    : url_(url),
      origin_(origin),
      location_(location),
      protocol_(protocol),
      mode_(MODE_INCOMPLETE) {
}
