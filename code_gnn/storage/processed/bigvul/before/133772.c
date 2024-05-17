int MapOpenSSLError(int err, const crypto::OpenSSLErrStackTracer& tracer) {
  switch (err) {
    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      return ERR_IO_PENDING;
    case SSL_ERROR_SYSCALL:
      LOG(ERROR) << "OpenSSL SYSCALL error, earliest error code in "
                    "error queue: " << ERR_peek_error() << ", errno: "
                 << errno;
      return ERR_SSL_PROTOCOL_ERROR;
    case SSL_ERROR_SSL:
      return MapOpenSSLErrorSSL();
    default:
      LOG(WARNING) << "Unknown OpenSSL error " << err;
      return ERR_SSL_PROTOCOL_ERROR;
   }
 }
