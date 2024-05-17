int GetPortFromSockaddr(const struct sockaddr* address, socklen_t address_len) {
  const uint16* port_field = GetPortFieldFromSockaddr(address, address_len);
  if (!port_field)
    return -1;
  return ntohs(*port_field);
}
