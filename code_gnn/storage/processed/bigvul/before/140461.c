int GetIdleSocketCountInSSLSocketPool(net::HttpNetworkSession* session) {
  return session->GetSSLSocketPool(
      net::HttpNetworkSession::NORMAL_SOCKET_POOL)->IdleSocketCount();
}
