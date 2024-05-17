int SSLClientSocketOpenSSL::DoHandshake() {
  crypto::OpenSSLErrStackTracer err_tracer(FROM_HERE);
  int net_error = net::OK;
  int rv = SSL_do_handshake(ssl_);

  if (client_auth_cert_needed_) {
    net_error = ERR_SSL_CLIENT_AUTH_CERT_NEEDED;
    if (rv == 1) {
      SSL_SESSION* session = SSL_get_session(ssl_);
      if (session) {
        int rv = SSL_CTX_remove_session(SSL_get_SSL_CTX(ssl_), session);
        LOG_IF(WARNING, !rv) << "Couldn't invalidate SSL session: " << session;
      }
    }
  } else if (rv == 1) {
    if (trying_cached_session_ && logging::DEBUG_MODE) {
      DVLOG(2) << "Result of session reuse for " << host_and_port_.ToString()
               << " is: " << (SSL_session_reused(ssl_) ? "Success" : "Fail");
    }
    const bool got_cert = !!UpdateServerCert();
    DCHECK(got_cert);
    net_log_.AddEvent(
        NetLog::TYPE_SSL_CERTIFICATES_RECEIVED,
        base::Bind(&NetLogX509CertificateCallback,
                   base::Unretained(server_cert_.get())));
    GotoState(STATE_VERIFY_CERT);
  } else {
    int ssl_error = SSL_get_error(ssl_, rv);

    if (ssl_error == SSL_ERROR_WANT_CHANNEL_ID_LOOKUP) {
      net_error = channel_id_request_return_value_;
    } else {
      net_error = MapOpenSSLError(ssl_error, err_tracer);
    }

    if (net_error == ERR_IO_PENDING) {
      GotoState(STATE_HANDSHAKE);
    } else {
      LOG(ERROR) << "handshake failed; returned " << rv
                 << ", SSL error code " << ssl_error
                 << ", net_error " << net_error;
      net_log_.AddEvent(
          NetLog::TYPE_SSL_HANDSHAKE_ERROR,
          CreateNetLogSSLErrorCallback(net_error, ssl_error));
    }
  }
  return net_error;
}
