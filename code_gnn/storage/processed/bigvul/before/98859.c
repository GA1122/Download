void WebSocketExperimentTask::OnClose(net::WebSocket* websocket) {
  RevokeTimeoutTimer();
  websocket_ = NULL;
  result_.websocket_total =
      base::TimeTicks::Now() - websocket_connect_start_time_;
  int result = net::ERR_CONNECTION_CLOSED;
  if (last_websocket_error_ != net::OK)
    result = last_websocket_error_;
  if (next_state_ == STATE_WEBSOCKET_CLOSE_COMPLETE)
    result = net::OK;
  DoLoop(result);
}
