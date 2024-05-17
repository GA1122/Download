int WebSocketExperimentTask::DoWebSocketKeepIdleComplete(int result) {
  if (result != net::ERR_TIMED_OUT) {
    if (result == net::OK)
      result = net::ERR_UNEXPECTED;
    return result;
  }

  DCHECK(websocket_);

  next_state_ = STATE_WEBSOCKET_RECV_PUSH_MESSAGE;
  SetTimeout(config_.websocket_receive_push_message_deadline_ms);
  return net::ERR_IO_PENDING;
}
