int WebSocketExperimentTask::DoWebSocketReceiveHello(int result) {
  if (result < 0)
    return result;

  DCHECK(websocket_);

  if (received_messages_.size() != 1)
    return net::ERR_INVALID_RESPONSE;

  std::string msg = received_messages_.front();
  received_messages_.pop_front();
  if (msg != config_.websocket_hello_message)
    return net::ERR_INVALID_RESPONSE;

  next_state_ = STATE_WEBSOCKET_KEEP_IDLE;
  return net::OK;
}
