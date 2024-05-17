void Resource::SetResponse(const ResourceResponse& response) {
  response_ = response;

  if (!GetResourceRequest().Url().ProtocolIsInHTTPFamily() ||
      !GetResponse().Url().ProtocolIsInHTTPFamily()) {
    cache_handler_.Clear();
    return;
  }

  if (GetResponse().WasFetchedViaServiceWorker()) {
    cache_handler_ = ServiceWorkerResponseCachedMetadataHandler::Create(
        this, fetcher_security_origin_.get());
  } else {
    cache_handler_ = CachedMetadataHandlerImpl::Create(this);
  }
}
