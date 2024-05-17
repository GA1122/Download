  std::unique_ptr<NavigationLoaderInterceptor> CreateServiceWorkerInterceptor(
      const NavigationRequestInfo& request_info,
      ServiceWorkerNavigationHandleCore* service_worker_navigation_handle_core)
      const {
    const ResourceType resource_type = request_info.is_main_frame
                                           ? RESOURCE_TYPE_MAIN_FRAME
                                           : RESOURCE_TYPE_SUB_FRAME;
    network::mojom::RequestContextFrameType frame_type =
        request_info.is_main_frame
            ? network::mojom::RequestContextFrameType::kTopLevel
            : network::mojom::RequestContextFrameType::kNested;
    storage::BlobStorageContext* blob_storage_context = GetBlobStorageContext(
        GetChromeBlobStorageContextForResourceContext(resource_context_));
    return ServiceWorkerRequestHandler::InitializeForNavigationNetworkService(
        resource_request_->url, resource_context_,
        service_worker_navigation_handle_core, blob_storage_context,
        request_info.begin_params->skip_service_worker, resource_type,
        request_info.begin_params->request_context_type, frame_type,
        request_info.are_ancestors_secure, request_info.common_params.post_data,
        web_contents_getter_);
  }
