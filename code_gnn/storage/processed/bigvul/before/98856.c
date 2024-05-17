int WebSocketExperimentTask::DoWebSocketReceivePushMessage(int result) {
  if (result < 0)
    return result;

  DCHECK(websocket_);
  if (received_messages_.size() != 1)
    return net::ERR_INVALID_RESPONSE;

  push_message_ = received_messages_.front();
  received_messages_.pop_front();

  next_state_ = STATE_WEBSOCKET_ECHO_BACK_MESSAGE;
  return net::OK;
}
