int HttpProxyClientSocket::DoReadHeaders() {
  next_state_ = STATE_READ_HEADERS_COMPLETE;
  return http_stream_parser_->ReadResponseHeaders(io_callback_);
}
