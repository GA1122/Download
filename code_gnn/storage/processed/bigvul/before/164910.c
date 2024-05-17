void ResourceDispatcherHostImpl::BeginNavigationRequest(
    ResourceContext* resource_context,
    net::URLRequestContext* request_context,
    storage::FileSystemContext* upload_file_system_context,
    const NavigationRequestInfo& info,
    std::unique_ptr<NavigationUIData> navigation_ui_data,
    network::mojom::URLLoaderClientPtr url_loader_client,
    network::mojom::URLLoaderRequest url_loader_request,
    ServiceWorkerNavigationHandleCore* service_worker_handle_core,
    AppCacheNavigationHandleCore* appcache_handle_core,
    uint32_t url_loader_options,
    net::RequestPriority net_priority,
    const GlobalRequestID& global_request_id) {
  DCHECK(url_loader_client.is_bound());
  DCHECK(url_loader_request.is_pending());

  ResourceType resource_type = info.is_main_frame ?
      RESOURCE_TYPE_MAIN_FRAME : RESOURCE_TYPE_SUB_FRAME;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  bool is_external_protocol =
      info.common_params.url.is_valid() &&
      !request_context->job_factory()->IsHandledProtocol(
          info.common_params.url.scheme());
  bool non_web_url_in_guest =
      info.is_for_guests_only &&
      !policy->IsWebSafeScheme(info.common_params.url.scheme()) &&
      !is_external_protocol;

  if (is_shutdown_ || non_web_url_in_guest) {
    url_loader_client->OnComplete(
        network::URLLoaderCompletionStatus(net::ERR_ABORTED));
    return;
  }

  std::unique_ptr<net::URLRequest> new_request;
  new_request = request_context->CreateRequest(
      info.common_params.url, net_priority, nullptr, GetTrafficAnnotation());

  new_request->set_method(info.common_params.method);
  new_request->set_site_for_cookies(info.site_for_cookies);
  new_request->set_top_frame_origin(info.top_frame_origin);
  new_request->set_initiator(info.common_params.initiator_origin);
  new_request->set_upgrade_if_insecure(info.upgrade_if_insecure);
  if (info.is_main_frame) {
    new_request->set_first_party_url_policy(
        net::URLRequest::UPDATE_FIRST_PARTY_URL_ON_REDIRECT);
  }

  std::unique_ptr<network::ScopedThrottlingToken> throttling_token =
      network::ScopedThrottlingToken::MaybeCreate(
          new_request->net_log().source().id, info.devtools_frame_token);

  Referrer::SetReferrerForRequest(new_request.get(),
                                  info.common_params.referrer);

  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(info.begin_params->headers);
  new_request->SetExtraRequestHeaders(headers);

  new_request->SetLoadFlags(info.begin_params->load_flags);

  storage::BlobStorageContext* blob_context = GetBlobStorageContext(
      GetChromeBlobStorageContextForResourceContext(resource_context));

  network::ResourceRequestBody* body = info.common_params.post_data.get();
  BlobHandles blob_handles;
  if (body) {
    if (!GetBodyBlobDataHandles(body, resource_context, &blob_handles)) {
      new_request->CancelWithError(net::ERR_INSUFFICIENT_RESOURCES);
      url_loader_client->OnComplete(
          network::URLLoaderCompletionStatus(net::ERR_ABORTED));
      return;
    }
    new_request->set_upload(UploadDataStreamBuilder::Build(
        body, blob_context, upload_file_system_context,
        base::CreateSingleThreadTaskRunnerWithTraits(
            {base::MayBlock(), base::TaskPriority::USER_VISIBLE})
            .get()));
  }

  ResourceRequestInfoImpl* extra_info = new ResourceRequestInfoImpl(
      ResourceRequesterInfo::CreateForBrowserSideNavigation(
          service_worker_handle_core
              ? service_worker_handle_core->context_wrapper()
              : scoped_refptr<ServiceWorkerContextWrapper>()),
      -1,   
      info.frame_tree_node_id,
      ChildProcessHost::kInvalidUniqueID,   
      global_request_id.request_id,
      -1,                       
      info.is_main_frame, {},   
      resource_type, info.common_params.transition,
      false,   
      false,   
      IsNavigationDownloadAllowed(info.common_params.download_policy),
      info.common_params.has_user_gesture,
      true,    
      false,   
      false,   
      false,   
      info.common_params.referrer.policy, info.is_prerendering,
      resource_context, info.report_raw_headers,
      info.report_raw_headers,
      true,   
      info.common_params.previews_state, info.common_params.post_data,
      false);   
  extra_info->SetBlobHandles(std::move(blob_handles));
  extra_info->set_navigation_ui_data(std::move(navigation_ui_data));

  extra_info->AssociateWithRequest(new_request.get());

  if (new_request->url().SchemeIs(url::kBlobScheme)) {
    storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
        new_request.get(),
        blob_context->GetBlobDataFromPublicURL(new_request->url()));
  }

  network::mojom::RequestContextFrameType frame_type =
      info.is_main_frame ? network::mojom::RequestContextFrameType::kTopLevel
                         : network::mojom::RequestContextFrameType::kNested;
  ServiceWorkerRequestHandler::InitializeForNavigation(
      new_request.get(), service_worker_handle_core, blob_context,
      info.begin_params->skip_service_worker, resource_type,
      info.begin_params->request_context_type, frame_type,
      info.are_ancestors_secure, info.common_params.post_data,
      extra_info->GetWebContentsGetterForRequest());

  if (appcache_handle_core) {
    AppCacheInterceptor::SetExtraRequestInfoForHost(
        new_request.get(), appcache_handle_core->host(), resource_type, false);
  }

  std::unique_ptr<ResourceHandler> handler;
  handler = std::make_unique<MojoAsyncResourceHandler>(
      new_request.get(), this, std::move(url_loader_request),
      std::move(url_loader_client), resource_type, url_loader_options);

  handler = AddStandardHandlers(
      new_request.get(), resource_type, resource_context,
      network::mojom::FetchRequestMode::kNoCors,
      info.begin_params->request_context_type, url_loader_options,
      appcache_handle_core ? appcache_handle_core->GetAppCacheService()
                           : nullptr,
      -1,   
      -1,   
      std::move(handler));

  BeginRequestInternal(std::move(new_request), std::move(handler),
                       false  ,
                       std::move(throttling_token));
}
