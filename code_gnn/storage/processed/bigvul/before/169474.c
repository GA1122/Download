bool IfaddrsToNetworkInterfaceList(int policy,
                                   const ifaddrs* interfaces,
                                   IPAttributesGetter* ip_attributes_getter,
                                   NetworkInterfaceList* networks) {
  for (const ifaddrs* interface = interfaces; interface != NULL;
       interface = interface->ifa_next) {
    if (!(IFF_RUNNING & interface->ifa_flags))
      continue;
    if (IFF_LOOPBACK & interface->ifa_flags)
      continue;
    struct sockaddr* addr = interface->ifa_addr;
    if (!addr)
      continue;

    if (IsLoopbackOrUnspecifiedAddress(addr))
      continue;

    std::string name = interface->ifa_name;
    if (ShouldIgnoreInterface(name, policy)) {
      continue;
    }

    NetworkChangeNotifier::ConnectionType connection_type =
        NetworkChangeNotifier::CONNECTION_UNKNOWN;

    int ip_attributes = IP_ADDRESS_ATTRIBUTE_NONE;

    if (ip_attributes_getter && ip_attributes_getter->IsInitialized()) {
      if (addr->sa_family == AF_INET6 &&
          ip_attributes_getter->GetAddressAttributes(interface,
                                                     &ip_attributes)) {
        if (ip_attributes &
            (IP_ADDRESS_ATTRIBUTE_ANYCAST | IP_ADDRESS_ATTRIBUTE_DUPLICATED |
             IP_ADDRESS_ATTRIBUTE_TENTATIVE | IP_ADDRESS_ATTRIBUTE_DETACHED)) {
          continue;
        }
      }

      connection_type =
          ip_attributes_getter->GetNetworkInterfaceType(interface);
    }

    IPEndPoint address;

    int addr_size = 0;
    if (addr->sa_family == AF_INET6) {
      addr_size = sizeof(sockaddr_in6);
    } else if (addr->sa_family == AF_INET) {
      addr_size = sizeof(sockaddr_in);
    }

    if (address.FromSockAddr(addr, addr_size)) {
      uint8_t prefix_length = 0;
      if (interface->ifa_netmask) {
        if (interface->ifa_netmask->sa_family == 0) {
          interface->ifa_netmask->sa_family = addr->sa_family;
        }
        IPEndPoint netmask;
        if (netmask.FromSockAddr(interface->ifa_netmask, addr_size)) {
          prefix_length = MaskPrefixLength(netmask.address());
        }
      }
      networks->push_back(NetworkInterface(
          name, name, if_nametoindex(name.c_str()), connection_type,
          address.address(), prefix_length, ip_attributes));
    }
  }

  return true;
}
