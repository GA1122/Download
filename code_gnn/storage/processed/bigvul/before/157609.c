bool IsTransportSocketPoolStalled(HttpNetworkSession* session) {
  return session->GetTransportSocketPool(HttpNetworkSession::NORMAL_SOCKET_POOL)
      ->IsStalled();
}
