RenderFrameImpl::BuildServiceWorkerNetworkProviderForNavigation(
    blink::mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info) {
  if (!provider_info) {
    return ServiceWorkerNetworkProviderForFrame::CreateInvalidInstance();
  }
  scoped_refptr<network::SharedURLLoaderFactory> fallback_factory =
      network::SharedURLLoaderFactory::Create(
          GetLoaderFactoryBundle()->CloneWithoutAppCacheFactory());
  return ServiceWorkerNetworkProviderForFrame::Create(
      this, std::move(provider_info), std::move(controller_service_worker_info),
      std::move(fallback_factory));
}
