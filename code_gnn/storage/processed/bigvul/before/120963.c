void SocketStream::Delegate::OnSSLCertificateError(
    SocketStream* socket,
    const SSLInfo& ssl_info,
    bool fatal) {
  socket->CancelWithSSLError(ssl_info);
}
