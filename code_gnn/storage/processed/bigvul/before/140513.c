NextProto HttpProxyClientSocket::GetNegotiatedProtocol() const {
  if (transport_.get() && transport_->socket()) {
    return transport_->socket()->GetNegotiatedProtocol();
  }
  NOTREACHED();
  return kProtoUnknown;
}
