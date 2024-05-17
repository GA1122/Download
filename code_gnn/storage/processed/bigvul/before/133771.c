bool SSLClientSocketOpenSSL::IsConnectedAndIdle() const {
  if (!completed_handshake_)
    return false;
  if (user_read_buf_.get() || user_write_buf_.get())
    return false;
  if (BIO_ctrl_pending(transport_bio_) > 0 ||
      BIO_ctrl_wpending(transport_bio_) > 0) {
    return false;
  }

  return transport_->socket()->IsConnectedAndIdle();
}
