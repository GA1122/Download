int SSLClientSocketOpenSSL::Connect(const CompletionCallback& callback) {
  net_log_.BeginEvent(NetLog::TYPE_SSL_CONNECT);

  if (!Init()) {
    int result = ERR_UNEXPECTED;
    net_log_.EndEventWithNetErrorCode(NetLog::TYPE_SSL_CONNECT, result);
    return result;
  }

  SSL_set_connect_state(ssl_);

  GotoState(STATE_HANDSHAKE);
  int rv = DoHandshakeLoop(net::OK);
  if (rv == ERR_IO_PENDING) {
    user_connect_callback_ = callback;
  } else {
    net_log_.EndEventWithNetErrorCode(NetLog::TYPE_SSL_CONNECT, rv);
  }

  return rv > OK ? OK : rv;
}
