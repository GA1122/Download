void SyncTest::DisableNetwork(Profile* profile) {
  DisableNotificationsImpl();
  net::ProxyConfig config;
  config.proxy_rules().ParseFromString("http=127.0.0.1:0");
  SetProxyConfig(profile->GetRequestContext(), config);
  net::NetworkChangeNotifier::NotifyObserversOfIPAddressChangeForTests();
}
