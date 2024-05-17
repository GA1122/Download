std::string NetAddressToString(const struct sockaddr* net_address,
                               socklen_t address_len) {
#if defined(OS_WIN)
  EnsureWinsockInit();
#endif

  char buffer[INET6_ADDRSTRLEN];

  int result = getnameinfo(net_address, address_len, buffer, sizeof(buffer),
                           NULL, 0, NI_NUMERICHOST);

  if (result != 0) {
    DVLOG(1) << "getnameinfo() failed with " << result << ": "
             << gai_strerror(result);
    buffer[0] = '\0';
  }
  return std::string(buffer);
}
