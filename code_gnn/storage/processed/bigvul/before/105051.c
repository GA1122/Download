void LiveSyncTest::EnableNetwork(Profile* profile) {
  SetProxyConfig(profile->GetRequestContext(),
                 net::ProxyConfig::CreateDirect());
  net::NetworkChangeNotifier::NotifyObserversOfIPAddressChangeForTests();
}
