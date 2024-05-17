RenderFrameImpl::CreateServiceWorkerProvider() {
  if (!frame_->GetDocumentLoader())
    return nullptr;

  if (!ChildThreadImpl::current())
    return nullptr;   
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromWebServiceWorkerNetworkProvider(
          frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider());
  if (!provider->context()) {
    return nullptr;
  }
  return std::make_unique<WebServiceWorkerProviderImpl>(provider->context());
}
