int GetIdleSocketCountInSSLSocketPool(HttpNetworkSession* session) {
  return session->GetSSLSocketPool(HttpNetworkSession::NORMAL_SOCKET_POOL)
      ->IdleSocketCount();
}
