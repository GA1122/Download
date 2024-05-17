sync_api::HttpPostProviderFactory* SyncBackendHost::MakeHttpBridgeFactory(
    net::URLRequestContextGetter* getter) {
  return new HttpBridgeFactory(getter);
}
