void NetworkChangeNotifier::NotifyObserversOfIPAddressChange() {
  if (g_network_change_notifier) {
    g_network_change_notifier->ip_address_observer_list_->Notify(
        &IPAddressObserver::OnIPAddressChanged);
  }
}
