int SSLClientSocketOpenSSL::DoPayloadRead() {
  crypto::OpenSSLErrStackTracer err_tracer(FROM_HERE);

  int rv;
  if (pending_read_error_ != kNoPendingReadResult) {
    rv = pending_read_error_;
    pending_read_error_ = kNoPendingReadResult;
    if (rv == 0) {
      net_log_.AddByteTransferEvent(NetLog::TYPE_SSL_SOCKET_BYTES_RECEIVED,
                                    rv, user_read_buf_->data());
    }
    return rv;
  }

  int total_bytes_read = 0;
  do {
    rv = SSL_read(ssl_, user_read_buf_->data() + total_bytes_read,
                  user_read_buf_len_ - total_bytes_read);
    if (rv > 0)
      total_bytes_read += rv;
  } while (total_bytes_read < user_read_buf_len_ && rv > 0);

  if (total_bytes_read == user_read_buf_len_) {
    rv = total_bytes_read;
  } else {
    int *next_result = &rv;
    if (total_bytes_read > 0) {
      pending_read_error_ = rv;
      rv = total_bytes_read;
      next_result = &pending_read_error_;
    }

    if (client_auth_cert_needed_) {
      *next_result = ERR_SSL_CLIENT_AUTH_CERT_NEEDED;
    } else if (*next_result < 0) {
      int err = SSL_get_error(ssl_, *next_result);
      *next_result = MapOpenSSLError(err, err_tracer);
      if (rv > 0 && *next_result == ERR_IO_PENDING) {
        *next_result = kNoPendingReadResult;
      }
    }
  }

  if (rv >= 0) {
    net_log_.AddByteTransferEvent(NetLog::TYPE_SSL_SOCKET_BYTES_RECEIVED, rv,
                                  user_read_buf_->data());
  }
  return rv;
}
