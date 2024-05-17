int WebSocketExperimentTask::DoWebSocketClose() {
  DCHECK(websocket_);

  next_state_ = STATE_WEBSOCKET_CLOSE_COMPLETE;
  websocket_->Close();
  SetTimeout(config_.websocket_close_deadline_ms);
  return net::ERR_IO_PENDING;
}
