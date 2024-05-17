uint16* GetPortFieldFromAddrinfo(struct addrinfo* info) {
  const struct addrinfo* const_info = info;
  const uint16* port_field = GetPortFieldFromAddrinfo(const_info);
  return const_cast<uint16*>(port_field);
}
