bool IPv6Supported() {
#if defined(OS_ANDROID)
  NOTIMPLEMENTED();
  return true;
#elif defined(OS_POSIX)
  int test_socket = socket(AF_INET6, SOCK_STREAM, 0);
  if (test_socket == -1) {
    IPv6SupportResults(IPV6_CANNOT_CREATE_SOCKETS);
    return false;
  }
  close(test_socket);

  struct ifaddrs* interface_addr = NULL;
  int rv = getifaddrs(&interface_addr);
  if (rv != 0) {
     IPv6SupportResults(IPV6_GETIFADDRS_FAILED);
     return true;   
  }

  bool found_ipv6 = false;
  for (struct ifaddrs* interface = interface_addr;
       interface != NULL;
       interface = interface->ifa_next) {
    if (!(IFF_UP & interface->ifa_flags))
      continue;
    if (IFF_LOOPBACK & interface->ifa_flags)
      continue;
    struct sockaddr* addr = interface->ifa_addr;
    if (!addr)
      continue;
    if (addr->sa_family != AF_INET6)
      continue;
    struct sockaddr_in6* addr_in6 =
        reinterpret_cast<struct sockaddr_in6*>(addr);
    struct in6_addr* sin6_addr = &addr_in6->sin6_addr;
    if (IN6_IS_ADDR_LOOPBACK(sin6_addr) || IN6_IS_ADDR_LINKLOCAL(sin6_addr))
      continue;
    found_ipv6 = true;
    break;
  }
  freeifaddrs(interface_addr);
  if (!found_ipv6) {
    IPv6SupportResults(IPV6_GLOBAL_ADDRESS_MISSING);
    return false;
  }

  IPv6SupportResults(IPV6_GLOBAL_ADDRESS_PRESENT);
  return true;
#elif defined(OS_WIN)
  EnsureWinsockInit();
  SOCKET test_socket = socket(AF_INET6, SOCK_STREAM, 0);
  if (test_socket == INVALID_SOCKET) {
    IPv6SupportResults(IPV6_CANNOT_CREATE_SOCKETS);
    return false;
  }
  closesocket(test_socket);

  ULONG adapters_size = 15000;
  scoped_ptr_malloc<IP_ADAPTER_ADDRESSES> adapters;
  ULONG error;
  int num_tries = 0;
  do {
    adapters.reset(
        reinterpret_cast<PIP_ADAPTER_ADDRESSES>(malloc(adapters_size)));
    error = GetAdaptersAddresses(AF_UNSPEC,
                                 GAA_FLAG_SKIP_ANYCAST |
                                 GAA_FLAG_SKIP_MULTICAST |
                                 GAA_FLAG_SKIP_DNS_SERVER |
                                 GAA_FLAG_SKIP_FRIENDLY_NAME,
                                 NULL, adapters.get(), &adapters_size);
    num_tries++;
  } while (error == ERROR_BUFFER_OVERFLOW && num_tries <= 3);
  if (error == ERROR_NO_DATA) {
    IPv6SupportResults(IPV6_GLOBAL_ADDRESS_MISSING);
    return false;
  }
  if (error != ERROR_SUCCESS) {
    IPv6SupportResults(IPV6_GETIFADDRS_FAILED);
    return true;   
  }

  PIP_ADAPTER_ADDRESSES adapter;
  for (adapter = adapters.get(); adapter; adapter = adapter->Next) {
    if (adapter->OperStatus != IfOperStatusUp)
      continue;
    if (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
      continue;
    PIP_ADAPTER_UNICAST_ADDRESS unicast_address;
    for (unicast_address = adapter->FirstUnicastAddress;
         unicast_address;
         unicast_address = unicast_address->Next) {
      if (unicast_address->Address.lpSockaddr->sa_family != AF_INET6)
        continue;
      struct sockaddr_in6* addr_in6 = reinterpret_cast<struct sockaddr_in6*>(
          unicast_address->Address.lpSockaddr);
      struct in6_addr* sin6_addr = &addr_in6->sin6_addr;
      if (IN6_IS_ADDR_LOOPBACK(sin6_addr) || IN6_IS_ADDR_LINKLOCAL(sin6_addr))
        continue;
      IPv6SupportResults(IPV6_GLOBAL_ADDRESS_PRESENT);
      return true;
    }
  }

  IPv6SupportResults(IPV6_GLOBAL_ADDRESS_MISSING);
  return false;
#else
  NOTIMPLEMENTED();
  return true;
#endif   
}
