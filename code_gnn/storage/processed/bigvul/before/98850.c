int WebSocketExperimentTask::DoWebSocketConnectComplete(int result) {
  if (result < 0)
    return result;
  DCHECK(websocket_);

  next_state_ = STATE_WEBSOCKET_SEND_HELLO;
  return net::OK;
}
