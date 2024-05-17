bool HttpProxyClientSocket::WasNpnNegotiated() const {
  if (transport_.get() && transport_->socket()) {
    return transport_->socket()->WasNpnNegotiated();
  }
  NOTREACHED();
  return false;
}
