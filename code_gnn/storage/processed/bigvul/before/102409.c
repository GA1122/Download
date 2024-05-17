uint16 GetPortFromAddrinfo(const struct addrinfo* info) {
  const uint16* port_field = GetPortFieldFromAddrinfo(info);
  if (!port_field)
    return -1;
  return ntohs(*port_field);
}
