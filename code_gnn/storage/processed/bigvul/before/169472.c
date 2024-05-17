IPAttributesGetterMac::GetNetworkInterfaceType(const ifaddrs* if_addr) {
  if (!IsInitialized())
    return NetworkChangeNotifier::CONNECTION_UNKNOWN;

  struct ifmediareq ifmr = {};
  strncpy(ifmr.ifm_name, if_addr->ifa_name, sizeof(ifmr.ifm_name) - 1);

  if (ioctl(ioctl_socket_, SIOCGIFMEDIA, &ifmr) != -1) {
    if (ifmr.ifm_current & IFM_IEEE80211) {
      return NetworkChangeNotifier::CONNECTION_WIFI;
    }
    if (ifmr.ifm_current & IFM_ETHER) {
      return NetworkChangeNotifier::CONNECTION_ETHERNET;
    }
  }

  return NetworkChangeNotifier::CONNECTION_UNKNOWN;
}
