int HttpProxyClientSocket::DidDrainBodyForAuthRestart(bool keep_alive) {
  if (keep_alive && transport_->socket()->IsConnectedAndIdle()) {
    next_state_ = STATE_GENERATE_AUTH_TOKEN;
    transport_->set_reuse_type(ClientSocketHandle::REUSED_IDLE);
  } else {
    next_state_ = STATE_TCP_RESTART;
    transport_->socket()->Disconnect();
  }

  drain_buf_ = NULL;
  parser_buf_ = NULL;
  http_stream_parser_.reset();
  request_line_.clear();
  request_headers_.Clear();
  response_ = HttpResponseInfo();
  return OK;
}
