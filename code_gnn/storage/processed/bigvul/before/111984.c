void SyncTest::EnableNetwork(Profile* profile) {
  SetProxyConfig(profile->GetRequestContext(),
                 net::ProxyConfig::CreateDirect());
  if (notifications_enabled_) {
    EnableNotificationsImpl();
  }
  net::NetworkChangeNotifier::NotifyObserversOfIPAddressChangeForTests();
}
