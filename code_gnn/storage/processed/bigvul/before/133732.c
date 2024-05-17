int SSLClientSocketOpenSSL::BufferRecv(void) {
  if (transport_recv_busy_)
    return ERR_IO_PENDING;

  size_t requested = BIO_ctrl_get_read_request(transport_bio_);
  if (requested == 0) {
    return ERR_IO_PENDING;
  }

  size_t max_write = BIO_ctrl_get_write_guarantee(transport_bio_);
  if (!max_write)
    return ERR_IO_PENDING;

  recv_buffer_ = new IOBuffer(max_write);
  int rv = transport_->socket()->Read(
      recv_buffer_.get(),
      max_write,
      base::Bind(&SSLClientSocketOpenSSL::BufferRecvComplete,
                 base::Unretained(this)));
  if (rv == ERR_IO_PENDING) {
    transport_recv_busy_ = true;
  } else {
    rv = TransportReadComplete(rv);
  }
  return rv;
}
