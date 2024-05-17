bool RenderFrameImpl::IsControlledByServiceWorker() {
  blink::WebServiceWorkerNetworkProvider* web_provider =
      frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider();
  if (!web_provider)
    return false;
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromWebServiceWorkerNetworkProvider(
          web_provider);
  return provider->IsControlledByServiceWorker();
}
