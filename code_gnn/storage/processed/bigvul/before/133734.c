int SSLClientSocketOpenSSL::BufferSend(void) {
  if (transport_send_busy_)
    return ERR_IO_PENDING;

  if (!send_buffer_.get()) {
    size_t max_read = BIO_ctrl_pending(transport_bio_);
    if (!max_read)
      return 0;   
    send_buffer_ = new DrainableIOBuffer(new IOBuffer(max_read), max_read);
    int read_bytes = BIO_read(transport_bio_, send_buffer_->data(), max_read);
    DCHECK_GT(read_bytes, 0);
    CHECK_EQ(static_cast<int>(max_read), read_bytes);
  }

  int rv = transport_->socket()->Write(
      send_buffer_.get(),
      send_buffer_->BytesRemaining(),
      base::Bind(&SSLClientSocketOpenSSL::BufferSendComplete,
                 base::Unretained(this)));
  if (rv == ERR_IO_PENDING) {
    transport_send_busy_ = true;
  } else {
    TransportWriteComplete(rv);
  }
  return rv;
}
