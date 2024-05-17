const uint16* GetPortFieldFromAddrinfo(const struct addrinfo* info) {
  DCHECK(info);
  const struct sockaddr* address = info->ai_addr;
  DCHECK(address);
  DCHECK_EQ(info->ai_family, address->sa_family);
  return GetPortFieldFromSockaddr(address, info->ai_addrlen);
}
