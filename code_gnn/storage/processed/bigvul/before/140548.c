const HttpResponseInfo* SpdyProxyClientSocket::GetConnectResponseInfo() const {
  return response_.headers.get() ? &response_ : NULL;
}
