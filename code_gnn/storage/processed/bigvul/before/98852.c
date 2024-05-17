int WebSocketExperimentTask::DoWebSocketKeepIdle() {
  DCHECK(websocket_);

  next_state_ = STATE_WEBSOCKET_KEEP_IDLE_COMPLETE;
  websocket_idle_start_time_ = base::TimeTicks::Now();
  SetTimeout(config_.websocket_idle_ms);
  return net::ERR_IO_PENDING;
}
