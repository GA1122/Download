int GetIdleSocketCountInTransportSocketPool(HttpNetworkSession* session) {
  return session->GetTransportSocketPool(HttpNetworkSession::NORMAL_SOCKET_POOL)
      ->IdleSocketCount();
}
