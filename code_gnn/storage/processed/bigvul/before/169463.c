void NetworkChangeNotifierMac::ReachabilityCallback(
    SCNetworkReachabilityRef target,
    SCNetworkConnectionFlags flags,
    void* notifier) {
  NetworkChangeNotifierMac* notifier_mac =
      static_cast<NetworkChangeNotifierMac*>(notifier);

  DCHECK_EQ(notifier_mac->run_loop_.get(), CFRunLoopGetCurrent());

  ConnectionType new_type = CalculateConnectionType(flags);
  ConnectionType old_type;
  {
    base::AutoLock lock(notifier_mac->connection_type_lock_);
    old_type = notifier_mac->connection_type_;
    notifier_mac->connection_type_ = new_type;
  }
  if (old_type != new_type) {
    NotifyObserversOfConnectionTypeChange();
    double max_bandwidth_mbps =
        NetworkChangeNotifier::GetMaxBandwidthForConnectionSubtype(
            new_type == CONNECTION_NONE ? SUBTYPE_NONE : SUBTYPE_UNKNOWN);
    NotifyObserversOfMaxBandwidthChange(max_bandwidth_mbps, new_type);
  }

#if defined(OS_IOS)
  NotifyObserversOfIPAddressChange();
#endif   
}
