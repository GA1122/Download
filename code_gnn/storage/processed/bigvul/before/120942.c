int SocketStream::DoSOCKSConnect() {
  DCHECK_EQ(kSOCKSProxy, proxy_mode_);

  next_state_ = STATE_SOCKS_CONNECT_COMPLETE;

  StreamSocket* s = socket_.release();
  HostResolver::RequestInfo req_info(HostPortPair::FromURL(url_));

  DCHECK(!proxy_info_.is_empty());
  if (proxy_info_.proxy_server().scheme() == ProxyServer::SCHEME_SOCKS5)
    s = new SOCKS5ClientSocket(s, req_info);
  else
    s = new SOCKSClientSocket(s, req_info, context_->host_resolver());
  socket_.reset(s);
  metrics_->OnCountConnectionType(SocketStreamMetrics::SOCKS_CONNECTION);
  return socket_->Connect(io_callback_);
}
