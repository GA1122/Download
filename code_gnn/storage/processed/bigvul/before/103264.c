void WebSocketJob::SaveCookiesAndNotifyHeaderComplete() {
  DCHECK(handshake_response_->HasResponse());

  response_cookies_.clear();
  response_cookies_save_index_ = 0;

  handshake_response_->GetHeaders(
      kSetCookieHeaders, arraysize(kSetCookieHeaders), &response_cookies_);

  SaveNextCookie();
}
