void ConfigureServiceWorkerContextOnIO() {
  std::set<std::string> headers;
  headers.insert(kDevToolsEmulateNetworkConditionsClientId);
  content::ServiceWorkerContext::AddExcludedHeadersForFetchEvent(headers);
}
