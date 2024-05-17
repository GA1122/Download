NextProto SpdyProxyClientSocket::GetProtocolNegotiated() const {
  SSLInfo ssl_info;
  bool was_npn_negotiated;
  NextProto protocol_negotiated;
  spdy_stream_->GetSSLInfo(&ssl_info, &was_npn_negotiated,
                           &protocol_negotiated);
  return protocol_negotiated;
}
