void SpdyProxyClientSocket::LogBlockedTunnelResponse() const {
  ProxyClientSocket::LogBlockedTunnelResponse(
      response_.headers->response_code(),
      request_.url,
        true);
}
