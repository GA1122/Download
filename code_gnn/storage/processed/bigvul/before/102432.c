std::string NetAddressToStringWithPort(const struct sockaddr* net_address,
                                       socklen_t address_len) {
  std::string ip_address_string = NetAddressToString(net_address, address_len);
  if (ip_address_string.empty())
    return std::string();   

  int port = GetPortFromSockaddr(net_address, address_len);

  if (ip_address_string.find(':') != std::string::npos) {
    return base::StringPrintf("[%s]:%d", ip_address_string.c_str(), port);
  }

  return base::StringPrintf("%s:%d", ip_address_string.c_str(), port);
}
