void SSLClientSocketOpenSSL::Disconnect() {
  if (ssl_) {
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
    ssl_ = NULL;
  }
  if (transport_bio_) {
    BIO_free_all(transport_bio_);
    transport_bio_ = NULL;
  }

  verifier_.reset();
  transport_->socket()->Disconnect();

  transport_send_busy_ = false;
  send_buffer_ = NULL;
  transport_recv_busy_ = false;
  transport_recv_eof_ = false;
  recv_buffer_ = NULL;

  user_connect_callback_.Reset();
  user_read_callback_.Reset();
  user_write_callback_.Reset();
  user_read_buf_         = NULL;
  user_read_buf_len_     = 0;
  user_write_buf_        = NULL;
  user_write_buf_len_    = 0;

  pending_read_error_ = kNoPendingReadResult;
  transport_write_error_ = OK;

  server_cert_verify_result_.Reset();
  completed_handshake_ = false;

  cert_authorities_.clear();
  client_auth_cert_needed_ = false;
}
