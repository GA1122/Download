int SocketStream::DoGenerateProxyAuthToken() {
  next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE;
  if (!proxy_auth_controller_.get()) {
    DCHECK(context_);
    DCHECK(context_->http_transaction_factory());
    DCHECK(context_->http_transaction_factory()->GetSession());
    HttpNetworkSession* session =
        context_->http_transaction_factory()->GetSession();
    const char* scheme = proxy_info_.is_https() ? "https://" : "http://";
    GURL auth_url(scheme +
                  proxy_info_.proxy_server().host_port_pair().ToString());
    proxy_auth_controller_ =
        new HttpAuthController(HttpAuth::AUTH_PROXY,
                               auth_url,
                               session->http_auth_cache(),
                               session->http_auth_handler_factory());
  }
  HttpRequestInfo request_info;
  request_info.url = url_;
  request_info.method = "CONNECT";
  return proxy_auth_controller_->MaybeGenerateAuthToken(
      &request_info, io_callback_, net_log_);
}
