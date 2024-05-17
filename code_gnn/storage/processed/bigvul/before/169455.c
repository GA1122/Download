NetworkChangeNotifierMac::CalculateConnectionType(
    SCNetworkConnectionFlags flags) {
  bool reachable = CalculateReachability(flags);
  if (!reachable)
    return CONNECTION_NONE;

#if defined(OS_IOS)
  return (flags & kSCNetworkReachabilityFlagsIsWWAN) ? CONNECTION_3G
                                                     : CONNECTION_WIFI;
#else
  return ConnectionTypeFromInterfaces();
#endif
}
