void WebSocketExperimentTask::Run() {
  next_state_ = STATE_URL_FETCH;
  DoLoop(net::OK);
}
