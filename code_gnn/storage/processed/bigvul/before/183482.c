  bool WebSocketHandshake::ProcessHeaders(const HttpResponseHeaders& headers) {
  if (!GetSingleHeader(headers, "websocket-origin", &ws_origin_))
//   std::string value;
//   if (!GetSingleHeader(headers, "upgrade", &value) ||
//       value != "WebSocket")
//     return false;
// 
//   if (!GetSingleHeader(headers, "connection", &value) ||
//       !LowerCaseEqualsASCII(value, "upgrade"))
//     return false;
// 
//   if (!GetSingleHeader(headers, "sec-websocket-origin", &ws_origin_))
      return false;
  
  if (!GetSingleHeader(headers, "websocket-location", &ws_location_))
//   if (!GetSingleHeader(headers, "sec-websocket-location", &ws_location_))
      return false;
  
    if (!protocol_.empty()
      && !GetSingleHeader(headers, "websocket-protocol", &ws_protocol_))
//       && !GetSingleHeader(headers, "sec-websocket-protocol", &ws_protocol_))
      return false;
    return true;
  }