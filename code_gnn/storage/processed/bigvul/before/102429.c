std::string NetAddressToString(const struct addrinfo* net_address) {
  return NetAddressToString(net_address->ai_addr, net_address->ai_addrlen);
}
