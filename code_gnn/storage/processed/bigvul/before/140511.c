const HttpResponseInfo* HttpProxyClientSocket::GetConnectResponseInfo() const {
  return response_.headers.get() ? &response_ : NULL;
}
