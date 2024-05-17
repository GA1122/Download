void WebSocketJob::SaveNextCookie() {
  if (response_cookies_save_index_ == response_cookies_.size()) {
    response_cookies_.clear();
    response_cookies_save_index_ = 0;

    handshake_response_->RemoveHeaders(
        kSetCookieHeaders, arraysize(kSetCookieHeaders));
    std::string received_data = handshake_response_->GetResponse();
    while (receive_frame_handler_->UpdateCurrentBuffer(false) > 0) {
      received_data +=
          std::string(receive_frame_handler_->GetCurrentBuffer()->data(),
                      receive_frame_handler_->GetCurrentBufferSize());
      receive_frame_handler_->ReleaseCurrentBuffer();
    }

    state_ = OPEN;
    if (delegate_)
      delegate_->OnReceivedData(
          socket_, received_data.data(), received_data.size());

    handshake_response_.reset();

    WebSocketThrottle::GetInstance()->RemoveFromQueue(this);
    WebSocketThrottle::GetInstance()->WakeupSocketIfNecessary();
    return;
  }

  bool allow = true;
  CookieOptions options;
  GURL url = GetURLForCookies();
  std::string cookie = response_cookies_[response_cookies_save_index_];
  if (delegate_ && !delegate_->CanSetCookie(socket_, url, cookie, &options))
    allow = false;

  if (socket_ && delegate_ && state_ == CONNECTING) {
    if (allow && socket_->context()->cookie_store()) {
      options.set_include_httponly();
      socket_->context()->cookie_store()->SetCookieWithOptions(
          url, cookie, options);
    }
    response_cookies_save_index_++;
    SaveNextCookie();
  }
}
