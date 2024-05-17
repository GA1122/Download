RenderFrameImpl::CreateServiceWorkerProvider() {
  if (!frame_->GetDocumentLoader())
    return nullptr;

  if (!ChildThreadImpl::current())
    return nullptr;   
  ServiceWorkerNetworkProviderForFrame* provider =
      static_cast<ServiceWorkerNetworkProviderForFrame*>(
          frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider());
  if (!provider->context()) {
    return nullptr;
  }
  return std::make_unique<WebServiceWorkerProviderImpl>(provider->context());
}
