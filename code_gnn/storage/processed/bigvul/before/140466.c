bool IsTransportSocketPoolStalled(net::HttpNetworkSession* session) {
  return session->GetTransportSocketPool(
      net::HttpNetworkSession::NORMAL_SOCKET_POOL)->IsStalled();
}
