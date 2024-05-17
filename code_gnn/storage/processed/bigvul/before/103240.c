void WebSocketJob::AddCookieHeaderAndSend() {
  bool allow = true;
  if (delegate_ && !delegate_->CanGetCookies(socket_, GetURLForCookies()))
    allow = false;

  if (socket_ && delegate_ && state_ == CONNECTING) {
    handshake_request_->RemoveHeaders(
        kCookieHeaders, arraysize(kCookieHeaders));
    if (allow) {
      if (socket_->context()->cookie_store()) {
        CookieOptions cookie_options;
        cookie_options.set_include_httponly();
        std::string cookie =
            socket_->context()->cookie_store()->GetCookiesWithOptions(
                GetURLForCookies(), cookie_options);
        if (!cookie.empty())
          handshake_request_->AppendHeaderIfMissing("Cookie", cookie);
      }
    }

    if (spdy_websocket_stream_.get()) {
      linked_ptr<spdy::SpdyHeaderBlock> headers(new spdy::SpdyHeaderBlock);
      handshake_request_->GetRequestHeaderBlock(
          socket_->url(), headers.get(), &challenge_);
      spdy_websocket_stream_->SendRequest(headers);
    } else {
      const std::string& handshake_request =
          handshake_request_->GetRawRequest();
      handshake_request_sent_ = 0;
      socket_->net_log()->AddEvent(
          NetLog::TYPE_WEB_SOCKET_SEND_REQUEST_HEADERS,
          make_scoped_refptr(
              new NetLogWebSocketHandshakeParameter(handshake_request)));
      socket_->SendData(handshake_request.data(),
                        handshake_request.size());
    }
  }
}
