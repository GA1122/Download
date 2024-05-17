int SocketStream::DoWriteTunnelHeaders() {
  DCHECK_EQ(kTunnelProxy, proxy_mode_);

  next_state_ = STATE_WRITE_TUNNEL_HEADERS_COMPLETE;

  if (!tunnel_request_headers_.get()) {
    metrics_->OnCountConnectionType(SocketStreamMetrics::TUNNEL_CONNECTION);
    tunnel_request_headers_ = new RequestHeaders();
    tunnel_request_headers_bytes_sent_ = 0;
  }
  if (tunnel_request_headers_->headers_.empty()) {
    HttpRequestHeaders request_headers;
    request_headers.SetHeader("Host", GetHostAndOptionalPort(url_));
    request_headers.SetHeader("Proxy-Connection", "keep-alive");
    if (proxy_auth_controller_.get() && proxy_auth_controller_->HaveAuth())
      proxy_auth_controller_->AddAuthorizationHeader(&request_headers);
    tunnel_request_headers_->headers_ = base::StringPrintf(
        "CONNECT %s HTTP/1.1\r\n"
        "%s",
        GetHostAndPort(url_).c_str(),
        request_headers.ToString().c_str());
  }
  tunnel_request_headers_->SetDataOffset(tunnel_request_headers_bytes_sent_);
  int buf_len = static_cast<int>(tunnel_request_headers_->headers_.size() -
                                 tunnel_request_headers_bytes_sent_);
  DCHECK_GT(buf_len, 0);
  return socket_->Write(tunnel_request_headers_.get(), buf_len, io_callback_);
}
