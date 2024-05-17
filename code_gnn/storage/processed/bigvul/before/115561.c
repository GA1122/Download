void NetworkChangeNotifier::NotifyObserversOfOnlineStateChange() {
  if (g_network_change_notifier) {
    g_network_change_notifier->online_state_observer_list_->Notify(
        &OnlineStateObserver::OnOnlineStateChanged, !IsOffline());
  }
}
