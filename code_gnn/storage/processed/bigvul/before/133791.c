bool SSLClientSocketOpenSSL::WasEverUsed() const {
  if (transport_.get() && transport_->socket())
    return transport_->socket()->WasEverUsed();

  NOTREACHED();
  return false;
}
