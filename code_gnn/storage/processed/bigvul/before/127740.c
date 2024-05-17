bool MockNetworkTransaction::GetRemoteEndpoint(IPEndPoint* endpoint) const {
  *endpoint = IPEndPoint(IPAddress(127, 0, 0, 1), 80);
  return true;
}
