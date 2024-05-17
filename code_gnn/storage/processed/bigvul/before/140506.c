int HttpProxyClientSocket::DoSendRequest() {
  next_state_ = STATE_SEND_REQUEST_COMPLETE;

  if (request_line_.empty()) {
    DCHECK(request_headers_.IsEmpty());
    HttpRequestHeaders authorization_headers;
    if (auth_->HaveAuth())
      auth_->AddAuthorizationHeader(&authorization_headers);
    if (proxy_delegate_) {
      proxy_delegate_->OnBeforeTunnelRequest(proxy_server_,
                                             &authorization_headers);
    }
    BuildTunnelRequest(request_, authorization_headers, endpoint_,
                       &request_line_, &request_headers_);

    net_log_.AddEvent(
        NetLog::TYPE_HTTP_TRANSACTION_SEND_TUNNEL_HEADERS,
        base::Bind(&HttpRequestHeaders::NetLogCallback,
                   base::Unretained(&request_headers_),
                   &request_line_));
  }

  parser_buf_ = new GrowableIOBuffer();
  http_stream_parser_.reset(new HttpStreamParser(
      transport_.get(), &request_, parser_buf_.get(), net_log_));
  return http_stream_parser_->SendRequest(
      request_line_, request_headers_, &response_, io_callback_);
}
