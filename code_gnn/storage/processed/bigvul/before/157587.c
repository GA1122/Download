void AddWebSocketHeaders(HttpRequestHeaders* headers) {
  headers->SetHeader("Connection", "Upgrade");
  headers->SetHeader("Upgrade", "websocket");
  headers->SetHeader("Origin", "http://www.example.org");
  headers->SetHeader("Sec-WebSocket-Version", "13");
}
