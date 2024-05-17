bool SSLClientSocketOpenSSL::IsConnected() const {
  if (!completed_handshake_)
    return false;
  if (user_read_buf_.get() || user_write_buf_.get())
    return true;

  return transport_->socket()->IsConnected();
}
