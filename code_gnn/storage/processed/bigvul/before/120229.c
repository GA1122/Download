void LayerTreeHost::InitializeForTesting(scoped_ptr<Proxy> proxy_for_testing) {
  InitializeProxy(proxy_for_testing.Pass());
}
