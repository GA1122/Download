bool SpdyProxyClientSocket::GetSSLInfo(SSLInfo* ssl_info) {
  bool was_npn_negotiated;
  NextProto protocol_negotiated;
  return spdy_stream_->GetSSLInfo(ssl_info, &was_npn_negotiated,
                                  &protocol_negotiated);
}
