int WebSocketExperimentTask::DoWebSocketReceiveBye(int result) {
  if (result < 0)
    return result;

  DCHECK(websocket_);

  if (received_messages_.size() != 1)
    return net::ERR_INVALID_RESPONSE;

  std::string bye = received_messages_.front();
  received_messages_.pop_front();

  if (bye != config_.websocket_bye_message)
    return net::ERR_INVALID_RESPONSE;

  next_state_ = STATE_WEBSOCKET_CLOSE;
  return net::OK;
}
