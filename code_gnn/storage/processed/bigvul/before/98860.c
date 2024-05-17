void WebSocketExperimentTask::OnError(
    const net::WebSocket* websocket, int error) {
  DLOG(INFO) << "WebSocket error=" << net::ErrorToString(error);
  last_websocket_error_ = error;
}
