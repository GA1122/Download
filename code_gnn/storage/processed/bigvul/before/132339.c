int64_t RenderFrameImpl::serviceWorkerID(WebDataSource& data_source) {
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromDocumentState(
          DocumentState::FromDataSource(&data_source));
  if (provider->context() && provider->context()->controller())
    return provider->context()->controller()->version_id();
  return kInvalidServiceWorkerVersionId;
}
