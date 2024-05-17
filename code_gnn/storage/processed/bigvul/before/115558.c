NetworkChangeNotifier::NetworkChangeNotifier()
    : ip_address_observer_list_(
        new ObserverListThreadSafe<IPAddressObserver>(
            ObserverListBase<IPAddressObserver>::NOTIFY_EXISTING_ONLY)),
      online_state_observer_list_(
        new ObserverListThreadSafe<OnlineStateObserver>(
            ObserverListBase<OnlineStateObserver>::NOTIFY_EXISTING_ONLY)),
      resolver_state_observer_list_(
        new ObserverListThreadSafe<DNSObserver>(
            ObserverListBase<DNSObserver>::NOTIFY_EXISTING_ONLY)) {
  DCHECK(!g_network_change_notifier);
  g_network_change_notifier = this;
}
