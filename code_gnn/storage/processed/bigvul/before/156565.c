  static void CreateLoaderAndStart(
      RenderProcessHost* process,
      network::mojom::URLLoaderRequest request,
      int route_id,
      int request_id,
      const network::ResourceRequest& resource_request,
      network::mojom::URLLoaderClientPtrInfo client) {
    network::mojom::URLLoaderFactoryPtr factory;
    process->CreateURLLoaderFactory(mojo::MakeRequest(&factory));
    factory->CreateLoaderAndStart(
        std::move(request), route_id, request_id,
        network::mojom::kURLLoadOptionNone, resource_request,
        network::mojom::URLLoaderClientPtr(std::move(client)),
        net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS));
  }