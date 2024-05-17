HttpStream* SpdyProxyClientSocket::CreateConnectResponseStream() {
  return new ProxyConnectRedirectHttpStream(
      redirect_has_load_timing_info_ ? &redirect_load_timing_info_ : NULL);
}
