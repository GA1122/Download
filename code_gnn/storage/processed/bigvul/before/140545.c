int SpdyProxyClientSocket::DoSendRequest() {
  next_state_ = STATE_SEND_REQUEST_COMPLETE;

  HttpRequestHeaders authorization_headers;
  if (auth_->HaveAuth()) {
    auth_->AddAuthorizationHeader(&authorization_headers);
  }

  std::string request_line;
  HttpRequestHeaders request_headers;
  BuildTunnelRequest(request_, authorization_headers, endpoint_, &request_line,
                     &request_headers);

  net_log_.AddEvent(
      NetLog::TYPE_HTTP_TRANSACTION_SEND_TUNNEL_HEADERS,
      base::Bind(&HttpRequestHeaders::NetLogCallback,
                 base::Unretained(&request_headers),
                 &request_line));

  request_.extra_headers.MergeFrom(request_headers);
  scoped_ptr<SpdyHeaderBlock> headers(new SpdyHeaderBlock());
  CreateSpdyHeadersFromHttpRequest(request_, request_headers,
                                   spdy_stream_->GetProtocolVersion(), true,
                                   headers.get());
  if (spdy_stream_->GetProtocolVersion() > 2) {
    (*headers)[":path"] = endpoint_.ToString();
    headers->erase(":scheme");
  } else {
    (*headers)["url"] = endpoint_.ToString();
    headers->erase("scheme");
  }

  return spdy_stream_->SendRequestHeaders(headers.Pass(), MORE_DATA_TO_SEND);
}
