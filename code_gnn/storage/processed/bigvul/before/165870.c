RenderFrameImpl::BuildServiceWorkerNetworkProviderForNavigation(
    const RequestNavigationParams* request_params,
    blink::mojom::ControllerServiceWorkerInfoPtr
        controller_service_worker_info) {
  scoped_refptr<network::SharedURLLoaderFactory> fallback_factory =
      network::SharedURLLoaderFactory::Create(
          GetLoaderFactoryBundle()->CloneWithoutAppCacheFactory());
  return ServiceWorkerNetworkProvider::CreateForNavigation(
      routing_id_, request_params, frame_,
      std::move(controller_service_worker_info), std::move(fallback_factory));
}
