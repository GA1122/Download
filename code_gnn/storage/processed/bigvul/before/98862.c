void WebSocketExperimentTask::OnOpen(net::WebSocket* websocket) {
  result_.websocket_connect =
      base::TimeTicks::Now() - websocket_connect_start_time_;
  RevokeTimeoutTimer();
  int result = net::ERR_UNEXPECTED;
  if (next_state_ == STATE_WEBSOCKET_CONNECT_COMPLETE)
    result = net::OK;
  else
    DLOG(INFO) << "unexpected state=" << next_state_;
  DoLoop(result);
}
