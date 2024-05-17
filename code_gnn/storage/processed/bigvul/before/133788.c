void SSLClientSocketOpenSSL::TransportWriteComplete(int result) {
  DCHECK(ERR_IO_PENDING != result);
  if (result < 0) {
    DVLOG(1) << "TransportWriteComplete error " << result;
    (void)BIO_shutdown_wr(SSL_get_wbio(ssl_));

    transport_write_error_ = result;
    (void)BIO_shutdown_wr(transport_bio_);
    send_buffer_ = NULL;
  } else {
    DCHECK(send_buffer_.get());
    send_buffer_->DidConsume(result);
    DCHECK_GE(send_buffer_->BytesRemaining(), 0);
    if (send_buffer_->BytesRemaining() <= 0)
      send_buffer_ = NULL;
  }
}
