bool IPAttributesGetterMac::GetAddressAttributes(const ifaddrs* if_addr,
                                                 int* attributes) {
  struct in6_ifreq ifr = {};
  strncpy(ifr.ifr_name, if_addr->ifa_name, sizeof(ifr.ifr_name) - 1);
  memcpy(&ifr.ifr_ifru.ifru_addr, if_addr->ifa_addr, if_addr->ifa_addr->sa_len);
  int rv = ioctl(ioctl_socket_, SIOCGIFAFLAG_IN6, &ifr);
  if (rv >= 0) {
    *attributes = AddressFlagsToNetAddressAttributes(ifr.ifr_ifru.ifru_flags);
  }
  return (rv >= 0);
}
