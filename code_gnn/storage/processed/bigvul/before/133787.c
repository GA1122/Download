int SSLClientSocketOpenSSL::TransportReadComplete(int result) {
  DCHECK(ERR_IO_PENDING != result);
  if (result <= 0) {
    DVLOG(1) << "TransportReadComplete result " << result;
    if (result == 0)
      transport_recv_eof_ = true;
    (void)BIO_shutdown_wr(transport_bio_);
  } else if (transport_write_error_ < 0) {
    result = transport_write_error_;
  } else {
    DCHECK(recv_buffer_.get());
    int ret = BIO_write(transport_bio_, recv_buffer_->data(), result);
    base::debug::Alias(&result);
    base::debug::Alias(&ret);
    CHECK_EQ(result, ret);
  }
  recv_buffer_ = NULL;
  transport_recv_busy_ = false;
  return result;
}
