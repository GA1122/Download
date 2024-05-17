bool RenderFrameImpl::isControlledByServiceWorker(WebDataSource& data_source) {
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromDocumentState(
          DocumentState::FromDataSource(&data_source));
  if (!provider->context())
    return false;
  return provider->context()->controller_handle_id() !=
      kInvalidServiceWorkerHandleId;
}
