sync_api::HttpPostProviderFactory* SyncBackendHost::MakeHttpBridgeFactory(
    const scoped_refptr<net::URLRequestContextGetter>& getter) {
  return new HttpBridgeFactory(getter);
}
