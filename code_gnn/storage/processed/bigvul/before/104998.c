void HttpBridgeFactory::Destroy(sync_api::HttpPostProviderInterface* http) {
  static_cast<HttpBridge*>(http)->Release();
}
