RenderFrameImpl::CreateServiceWorkerProvider() {
  DCHECK(frame_->GetDocumentLoader());
  if (!ChildThreadImpl::current())
    return nullptr;   
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromWebServiceWorkerNetworkProvider(
          frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider());
  if (!provider->context()) {
    return nullptr;
  }
  return std::make_unique<WebServiceWorkerProviderImpl>(
      ChildThreadImpl::current()->thread_safe_sender(), provider->context());
}
