int WebSocketJob::TrySpdyStream() {
  if (!socket_.get())
    return ERR_FAILED;

  if (!websocket_over_spdy_enabled_)
    return OK;

  HttpTransactionFactory* factory =
      socket_->context()->http_transaction_factory();
  if (!factory)
    return OK;
  scoped_refptr<HttpNetworkSession> session = factory->GetSession();
  if (!session.get())
    return OK;
  SpdySessionPool* spdy_pool = session->spdy_session_pool();
  const HostPortProxyPair pair(HostPortPair::FromURL(socket_->url()),
                               socket_->proxy_server());
  if (!spdy_pool->HasSession(pair))
    return OK;

  scoped_refptr<SpdySession> spdy_session =
      spdy_pool->Get(pair, *socket_->net_log());
  SSLInfo ssl_info;
  bool was_npn_negotiated;
  bool use_ssl = spdy_session->GetSSLInfo(&ssl_info, &was_npn_negotiated);
  if (socket_->is_secure() && !use_ssl)
    return OK;

  spdy_websocket_stream_.reset(new SpdyWebSocketStream(spdy_session, this));

  int result = spdy_websocket_stream_->InitializeStream(
      socket_->url(), MEDIUM, *socket_->net_log());
  if (result == OK) {
    OnConnected(socket_, kMaxPendingSendAllowed);
    return ERR_PROTOCOL_SWITCHED;
  }
  if (result != ERR_IO_PENDING) {
    spdy_websocket_stream_.reset();
    return OK;
  }

  return ERR_IO_PENDING;
}
