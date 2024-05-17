void ResourceDispatcherHostImpl::BeginRequest(
    ResourceRequesterInfo* requester_info,
    int request_id,
    const network::ResourceRequest& request_data,
    bool is_sync_load,
    int route_id,
    uint32_t url_loader_options,
    network::mojom::URLLoaderRequest mojo_request,
    network::mojom::URLLoaderClientPtr url_loader_client,
    const net::NetworkTrafficAnnotationTag& traffic_annotation) {
  DCHECK(requester_info->IsRenderer() ||
         requester_info->IsNavigationPreload() ||
         requester_info->IsCertificateFetcherForSignedExchange());

  int child_id = requester_info->child_id();

  if (IsRequestIDInUse(GlobalRequestID(child_id, request_id))) {
    DCHECK(requester_info->IsRenderer());
    bad_message::ReceivedBadMessage(requester_info->filter(),
                                    bad_message::RDH_INVALID_REQUEST_ID);
    return;
  }

  if (IsResourceTypeFrame(
          static_cast<ResourceType>(request_data.resource_type))) {
    DCHECK(requester_info->IsRenderer());
    bad_message::ReceivedBadMessage(requester_info->filter(),
                                    bad_message::RDH_INVALID_URL);
    return;
  }

  if (request_data.priority < net::MINIMUM_PRIORITY ||
      request_data.priority > net::MAXIMUM_PRIORITY) {
    DCHECK(requester_info->IsRenderer());
    bad_message::ReceivedBadMessage(requester_info->filter(),
                                    bad_message::RDH_INVALID_PRIORITY);
    return;
  }

  DEBUG_ALIAS_FOR_GURL(url_buf, request_data.url);

  ResourceContext* resource_context = nullptr;
  net::URLRequestContext* request_context = nullptr;
  requester_info->GetContexts(
      static_cast<ResourceType>(request_data.resource_type), &resource_context,
      &request_context);

  if (is_shutdown_ ||
      !ShouldServiceRequest(child_id, request_data, request_data.headers,
                            requester_info, resource_context)) {
    AbortRequestBeforeItStarts(requester_info->filter(), request_id,
                               std::move(url_loader_client));
    return;
  }

  BlobHandles blob_handles;
  storage::BlobStorageContext* blob_context =
      GetBlobStorageContext(requester_info->blob_storage_context());
  if (request_data.request_body.get()) {
    if (blob_context) {
      if (!GetBodyBlobDataHandles(request_data.request_body.get(),
                                  resource_context, &blob_handles)) {
        AbortRequestBeforeItStarts(requester_info->filter(), request_id,
                                   std::move(url_loader_client));
        return;
      }
    }
  }

  for (net::HttpRequestHeaders::Iterator it(request_data.headers);
       it.GetNext();) {
    auto index = http_header_interceptor_map_.find(it.name());
    if (index != http_header_interceptor_map_.end()) {
      HeaderInterceptorInfo& interceptor_info = index->second;

      bool call_interceptor = true;
      if (!interceptor_info.starts_with.empty()) {
        call_interceptor =
            base::StartsWith(it.value(), interceptor_info.starts_with,
                             base::CompareCase::INSENSITIVE_ASCII);
      }
      if (call_interceptor) {
        interceptor_info.interceptor.Run(
            it.name(), it.value(), child_id, resource_context,
            base::Bind(
                &ResourceDispatcherHostImpl::ContinuePendingBeginRequest,
                base::Unretained(this), base::WrapRefCounted(requester_info),
                request_id, request_data, is_sync_load, route_id,
                request_data.headers, url_loader_options,
                base::Passed(std::move(mojo_request)),
                base::Passed(std::move(url_loader_client)),
                base::Passed(std::move(blob_handles)), traffic_annotation));
        return;
      }
    }
  }
  ContinuePendingBeginRequest(
      requester_info, request_id, request_data, is_sync_load, route_id,
      request_data.headers, url_loader_options, std::move(mojo_request),
      std::move(url_loader_client), std::move(blob_handles), traffic_annotation,
      HeaderInterceptorResult::CONTINUE);
}
