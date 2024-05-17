void SocketStream::CancelWithSSLError(const SSLInfo& ssl_info) {
  CancelWithError(MapCertStatusToNetError(ssl_info.cert_status));
}
