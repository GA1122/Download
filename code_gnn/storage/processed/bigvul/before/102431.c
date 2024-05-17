std::string NetAddressToStringWithPort(const struct addrinfo* net_address) {
  return NetAddressToStringWithPort(
      net_address->ai_addr, net_address->ai_addrlen);
}
