void SocketStream::DoRestartWithAuth() {
  DCHECK_EQ(next_state_, STATE_AUTH_REQUIRED);
  tunnel_request_headers_ = NULL;
  tunnel_request_headers_bytes_sent_ = 0;
  tunnel_response_headers_ = NULL;
  tunnel_response_headers_capacity_ = 0;
  tunnel_response_headers_len_ = 0;

  next_state_ = STATE_TCP_CONNECT;
  DoLoop(OK);
}
