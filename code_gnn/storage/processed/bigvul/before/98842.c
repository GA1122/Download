void WebSocketExperimentTask::Cancel() {
  next_state_ = STATE_NONE;
  DoLoop(net::OK);
}
