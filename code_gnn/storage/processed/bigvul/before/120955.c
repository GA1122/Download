int SocketStream::DoWriteTunnelHeadersComplete(int result) {
  DCHECK_EQ(kTunnelProxy, proxy_mode_);

  if (result < 0) {
    next_state_ = STATE_CLOSE;
    return result;
  }

  tunnel_request_headers_bytes_sent_ += result;
  if (tunnel_request_headers_bytes_sent_ <
      tunnel_request_headers_->headers_.size()) {
    next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
  } else {
    tunnel_request_headers_bytes_sent_ = 0;
    next_state_ = STATE_READ_TUNNEL_HEADERS;
  }
  return OK;
}
