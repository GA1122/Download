void ResourceDispatcherHostImpl::ContinuePendingBeginRequest(
    scoped_refptr<ResourceRequesterInfo> requester_info,
    int request_id,
    const network::ResourceRequest& request_data,
    bool is_sync_load,
    int route_id,
    const net::HttpRequestHeaders& headers,
    uint32_t url_loader_options,
    network::mojom::URLLoaderRequest mojo_request,
    network::mojom::URLLoaderClientPtr url_loader_client,
    BlobHandles blob_handles,
    const net::NetworkTrafficAnnotationTag& traffic_annotation,
    HeaderInterceptorResult interceptor_result) {
  DCHECK(requester_info->IsRenderer() ||
         requester_info->IsNavigationPreload() ||
         requester_info->IsCertificateFetcherForSignedExchange());
  DCHECK(!IsResourceTypeFrame(
      static_cast<ResourceType>(request_data.resource_type)));

  if (interceptor_result != HeaderInterceptorResult::CONTINUE) {
    if (requester_info->IsRenderer() &&
        interceptor_result == HeaderInterceptorResult::KILL) {
      bad_message::ReceivedBadMessage(requester_info->filter(),
                                      bad_message::RDH_ILLEGAL_ORIGIN);
    }
    AbortRequestBeforeItStarts(requester_info->filter(), request_id,
                               std::move(url_loader_client));
    return;
  }
  int child_id = requester_info->child_id();
  storage::BlobStorageContext* blob_context = nullptr;
  bool do_not_prompt_for_login = false;
  bool report_raw_headers = false;
  bool report_security_info = false;
  int load_flags = request_data.load_flags;

  ResourceContext* resource_context = nullptr;
  net::URLRequestContext* request_context = nullptr;
  requester_info->GetContexts(
      static_cast<ResourceType>(request_data.resource_type), &resource_context,
      &request_context);

  if (request_data.resource_type == RESOURCE_TYPE_PREFETCH &&
      request_data.method != "GET") {
    AbortRequestBeforeItStarts(requester_info->filter(), request_id,
                               std::move(url_loader_client));
    return;
  }

  std::unique_ptr<net::URLRequest> new_request = request_context->CreateRequest(
      request_data.url, request_data.priority, nullptr, traffic_annotation);

  if (requester_info->IsNavigationPreload()) {
    new_request->net_log().AddEvent(
        net::NetLogEventType::SERVICE_WORKER_NAVIGATION_PRELOAD_REQUEST);
  }

  new_request->set_method(request_data.method);
  new_request->set_site_for_cookies(request_data.site_for_cookies);
  new_request->set_top_frame_origin(request_data.top_frame_origin);
  new_request->set_attach_same_site_cookies(
      request_data.attach_same_site_cookies);
  new_request->set_upgrade_if_insecure(request_data.upgrade_if_insecure);

  DCHECK(request_data.request_initiator.has_value() ||
         IsResourceTypeFrame(
             static_cast<ResourceType>(request_data.resource_type)));
  new_request->set_initiator(request_data.request_initiator);

  if (request_data.originated_from_service_worker) {
    new_request->SetUserData(URLRequestServiceWorkerData::kUserDataKey,
                             std::make_unique<URLRequestServiceWorkerData>());
  }

  new_request->SetReferrer(network::ComputeReferrer(request_data.referrer));
  new_request->set_referrer_policy(request_data.referrer_policy);

  new_request->SetExtraRequestHeaders(headers);
  if (!request_data.requested_with_header.empty()) {
    new_request->SetExtraRequestHeaderByName(
        "X-Requested-With", request_data.requested_with_header, true);
  }
  if (!request_data.client_data_header.empty()) {
    new_request->SetExtraRequestHeaderByName(
        "X-Client-Data", request_data.client_data_header, true);
  }

  std::unique_ptr<network::ScopedThrottlingToken> throttling_token =
      network::ScopedThrottlingToken::MaybeCreate(
          new_request->net_log().source().id,
          request_data.throttling_profile_id);

  blob_context = GetBlobStorageContext(requester_info->blob_storage_context());
  if (request_data.request_body.get()) {
    new_request->set_upload(UploadDataStreamBuilder::Build(
        request_data.request_body.get(), blob_context,
        requester_info->file_system_context(),
        base::CreateSingleThreadTaskRunnerWithTraits(
            {base::MayBlock(), base::TaskPriority::USER_VISIBLE})
            .get()));
  }

  do_not_prompt_for_login = request_data.do_not_prompt_for_login;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  report_raw_headers = request_data.report_raw_headers;
  report_security_info = request_data.report_raw_headers;
  if (report_raw_headers && !policy->CanReadRawCookies(child_id) &&
      !requester_info->IsNavigationPreload()) {
    VLOG(1) << "Denied unauthorized request for raw headers";
    report_raw_headers = false;
  }

  if (report_raw_headers &&
      !policy->CanAccessDataForOrigin(child_id, request_data.url)) {
    report_raw_headers = false;
  }

  if (DoNotPromptForLogin(static_cast<ResourceType>(request_data.resource_type),
                          request_data.url, request_data.site_for_cookies)) {
    do_not_prompt_for_login = true;
    load_flags |= net::LOAD_DO_NOT_USE_EMBEDDED_IDENTITY;
  }

  if (is_sync_load) {
    DCHECK_EQ(request_data.priority, net::MAXIMUM_PRIORITY);
    DCHECK_NE(load_flags & net::LOAD_IGNORE_LIMITS, 0);
  } else {
    DCHECK_EQ(load_flags & net::LOAD_IGNORE_LIMITS, 0);
  }

  if (request_data.keepalive) {
    const auto& map = keepalive_statistics_recorder_.per_process_records();
    if (child_id != 0 && map.find(child_id) == map.end())
      keepalive_statistics_recorder_.Register(child_id);
  }

  new_request->SetLoadFlags(load_flags);

  ResourceRequestInfoImpl* extra_info = new ResourceRequestInfoImpl(
      requester_info, route_id,
      -1,   
      request_data.plugin_child_id, request_id, request_data.render_frame_id,
      request_data.is_main_frame,
      request_data.fetch_window_id ? *request_data.fetch_window_id
                                   : base::UnguessableToken(),
      static_cast<ResourceType>(request_data.resource_type),
      static_cast<ui::PageTransition>(request_data.transition_type),
      false,   
      false,   
      false,   
      request_data.has_user_gesture, request_data.enable_load_timing,
      request_data.enable_upload_progress, do_not_prompt_for_login,
      request_data.keepalive,
      Referrer::NetReferrerPolicyToBlinkReferrerPolicy(
          request_data.referrer_policy),
      request_data.is_prerendering, resource_context, report_raw_headers,
      report_security_info, !is_sync_load, request_data.previews_state,
      request_data.request_body, request_data.initiated_in_secure_context);
  extra_info->SetBlobHandles(std::move(blob_handles));

  extra_info->AssociateWithRequest(new_request.get());

  if (new_request->url().SchemeIs(url::kBlobScheme)) {
    storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
        new_request.get(), requester_info->blob_storage_context()
                               ->context()
                               ->GetBlobDataFromPublicURL(new_request->url()));
  }

  ServiceWorkerRequestHandler::InitializeHandler(
      new_request.get(), requester_info->service_worker_context(), blob_context,
      child_id, request_data.service_worker_provider_id,
      request_data.skip_service_worker, request_data.fetch_request_mode,
      request_data.fetch_credentials_mode, request_data.fetch_redirect_mode,
      request_data.fetch_integrity, request_data.keepalive,
      static_cast<ResourceType>(request_data.resource_type),
      static_cast<blink::mojom::RequestContextType>(
          request_data.fetch_request_context_type),
      request_data.fetch_frame_type, request_data.request_body);

  AppCacheInterceptor::SetExtraRequestInfo(
      new_request.get(), requester_info->appcache_service(), child_id,
      request_data.appcache_host_id,
      static_cast<ResourceType>(request_data.resource_type),
      request_data.should_reset_appcache);

  std::unique_ptr<ResourceHandler> handler = CreateResourceHandler(
      requester_info.get(), new_request.get(), request_data, route_id, child_id,
      resource_context, url_loader_options, std::move(mojo_request),
      std::move(url_loader_client));

  if (handler) {
    const bool is_initiated_by_fetch_api =
        request_data.fetch_request_context_type ==
        static_cast<int>(blink::mojom::RequestContextType::FETCH);
    BeginRequestInternal(std::move(new_request), std::move(handler),
                         is_initiated_by_fetch_api,
                         std::move(throttling_token));
  }
}
