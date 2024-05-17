RenderFrameImpl::IsControlledByServiceWorker() {
  blink::WebServiceWorkerNetworkProvider* web_provider =
      frame_->GetDocumentLoader()->GetServiceWorkerNetworkProvider();
  if (!web_provider)
    return blink::mojom::ControllerServiceWorkerMode::kNoController;
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromWebServiceWorkerNetworkProvider(
          web_provider);
  return provider->IsControlledByServiceWorker();
}
