void WebSocketExperimentTask::OnMessage(
    net::WebSocket* websocket, const std::string& msg) {
  if (!result_.websocket_echo.ToInternalValue())
    result_.websocket_echo =
        base::TimeTicks::Now() - websocket_echo_start_time_;
  if (!websocket_idle_start_time_.is_null() &&
      !result_.websocket_idle.ToInternalValue())
    result_.websocket_idle =
        base::TimeTicks::Now() - websocket_idle_start_time_;
  RevokeTimeoutTimer();
  received_messages_.push_back(msg);
  int result = net::ERR_UNEXPECTED;
  switch (next_state_) {
    case STATE_WEBSOCKET_RECV_HELLO:
    case STATE_WEBSOCKET_RECV_PUSH_MESSAGE:
    case STATE_WEBSOCKET_RECV_BYE:
      result = net::OK;
      break;
    default:
      DLOG(INFO) << "unexpected state=" << next_state_;
      break;
  }
  DoLoop(result);
}
