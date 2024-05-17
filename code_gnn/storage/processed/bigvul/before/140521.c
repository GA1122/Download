void HttpProxyClientSocket::LogBlockedTunnelResponse() const {
  ProxyClientSocket::LogBlockedTunnelResponse(
      response_.headers->response_code(),
      request_.url,
      is_https_proxy_);
}
