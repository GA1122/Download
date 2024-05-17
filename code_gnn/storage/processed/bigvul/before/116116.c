void ResourceDispatcherHostImpl::BeginRequest(
    int request_id,
    const ResourceHostMsg_Request& request_data,
    IPC::Message* sync_result,   
    int route_id) {
  ProcessType process_type = filter_->process_type();
  int child_id = filter_->child_id();

  char url_buf[128];
  base::strlcpy(url_buf, request_data.url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);

  net::URLRequest* deferred_request = NULL;

  GlobalRequestID old_request_id(request_data.transferred_request_child_id,
                                 request_data.transferred_request_request_id);
  TransferredNavigations::iterator iter =
      transferred_navigations_.find(old_request_id);
  if (iter != transferred_navigations_.end()) {
    deferred_request = iter->second;
    pending_requests_.erase(old_request_id);
    transferred_navigations_.erase(iter);
  }

  ResourceContext* resource_context = filter_->resource_context();
  CHECK(ContainsKey(active_resource_contexts_, resource_context));

  if (request_data.upload_data) {
    GetBlobStorageControllerForResourceContext(resource_context)->
        ResolveBlobReferencesInUploadData(request_data.upload_data.get());
  }

  if (is_shutdown_ ||
      !ShouldServiceRequest(process_type, child_id, request_data)) {
    AbortRequestBeforeItStarts(filter_, sync_result, route_id, request_id);
    return;
  }

  const Referrer referrer(MaybeStripReferrer(request_data.referrer),
                          request_data.referrer_policy);

  if (delegate_ && !delegate_->ShouldBeginRequest(child_id,
                                                  route_id,
                                                  request_data.method,
                                                  request_data.url,
                                                  request_data.resource_type,
                                                  resource_context,
                                                  referrer)) {
    AbortRequestBeforeItStarts(filter_, sync_result, route_id, request_id);
    return;
  }

  scoped_refptr<ResourceHandler> handler;
  if (sync_result) {
    handler = new SyncResourceHandler(
        filter_, request_data.url, sync_result, this);
  } else {
    handler = new AsyncResourceHandler(
        filter_, route_id, request_data.url, this);
  }

  if (request_data.download_to_file)
    handler = new RedirectToFileResourceHandler(handler, child_id, this);

  if (HandleExternalProtocol(
      request_id, child_id, route_id, request_data.url,
      request_data.resource_type,
      *resource_context->GetRequestContext()->job_factory(), handler)) {
    return;
  }

  net::URLRequest* request;
  if (deferred_request) {
    request = deferred_request;
  } else {
    request = new net::URLRequest(request_data.url, this);
    request->set_method(request_data.method);
    request->set_first_party_for_cookies(request_data.first_party_for_cookies);
    request->set_referrer(referrer.url.spec());
    webkit_glue::ConfigureURLRequestForReferrerPolicy(request, referrer.policy);
    net::HttpRequestHeaders headers;
    headers.AddHeadersFromString(request_data.headers);
    request->SetExtraRequestHeaders(headers);
  }

  int load_flags = request_data.load_flags;
  load_flags |= net::LOAD_VERIFY_EV_CERT;
  if (request_data.resource_type == ResourceType::MAIN_FRAME) {
    load_flags |= net::LOAD_MAIN_FRAME;
  } else if (request_data.resource_type == ResourceType::SUB_FRAME) {
    load_flags |= net::LOAD_SUB_FRAME;
  } else if (request_data.resource_type == ResourceType::PREFETCH) {
    load_flags |= (net::LOAD_PREFETCH | net::LOAD_DO_NOT_PROMPT_FOR_LOGIN);
  } else if (request_data.resource_type == ResourceType::FAVICON) {
    load_flags |= net::LOAD_DO_NOT_PROMPT_FOR_LOGIN;
  }

  if (sync_result)
    load_flags |= net::LOAD_IGNORE_LIMITS;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  if (!policy->CanUseCookiesForOrigin(child_id, request_data.url)) {
    load_flags |= (net::LOAD_DO_NOT_SEND_COOKIES |
                   net::LOAD_DO_NOT_SEND_AUTH_DATA |
                   net::LOAD_DO_NOT_SAVE_COOKIES);
  }

  if ((load_flags & net::LOAD_REPORT_RAW_HEADERS)
      && !policy->CanReadRawCookies(child_id)) {
    VLOG(1) << "Denied unauthorized request for raw headers";
    load_flags &= ~net::LOAD_REPORT_RAW_HEADERS;
  }

  request->set_load_flags(load_flags);

  request->set_context(
      filter_->GetURLRequestContext(request_data.resource_type));
  request->set_priority(DetermineRequestPriority(request_data.resource_type));

  uint64 upload_size = 0;
  if (request_data.upload_data) {
    request->set_upload(request_data.upload_data);
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    upload_size = request_data.upload_data->GetContentLengthSync();
  }

  if (request_data.resource_type == ResourceType::MAIN_FRAME &&
      process_type == PROCESS_TYPE_RENDERER &&
      CrossSiteRequestManager::GetInstance()->
          HasPendingCrossSiteRequest(child_id, route_id)) {
    handler = new CrossSiteResourceHandler(handler, child_id, route_id, this);
  }

  handler = new BufferedResourceHandler(handler, this, request);

  ScopedVector<ResourceThrottle> throttles;
  if (delegate_) {
    bool is_continuation_of_transferred_request =
        (deferred_request != NULL);

    delegate_->RequestBeginning(request,
                                resource_context,
                                request_data.resource_type,
                                child_id,
                                route_id,
                                is_continuation_of_transferred_request,
                                &throttles);
  }

  if (request_data.resource_type == ResourceType::MAIN_FRAME) {
    throttles.insert(
        throttles.begin(), new TransferNavigationResourceThrottle(request));
  }

  if (!throttles.empty()) {
    handler = new ThrottlingResourceHandler(this, handler, child_id, request_id,
                                            throttles.Pass());
  }

  bool allow_download = request_data.allow_download &&
      ResourceType::IsFrame(request_data.resource_type);
  ResourceRequestInfoImpl* extra_info =
      new ResourceRequestInfoImpl(
          handler,
          process_type,
          child_id,
          route_id,
          request_data.origin_pid,
          request_id,
          request_data.is_main_frame,
          request_data.frame_id,
          request_data.parent_is_main_frame,
          request_data.parent_frame_id,
          request_data.resource_type,
          request_data.transition_type,
          upload_size,
          false,   
          allow_download,
          request_data.has_user_gesture,
          request_data.referrer_policy,
          resource_context);
  extra_info->AssociateWithRequest(request);   

  if (request->url().SchemeIs(chrome::kBlobScheme)) {
    webkit_blob::BlobStorageController* controller =
        GetBlobStorageControllerForResourceContext(resource_context);
    extra_info->set_requested_blob_data(
        controller->GetBlobDataFromUrl(request->url()));
  }

  appcache::AppCacheInterceptor::SetExtraRequestInfo(
      request, ResourceContext::GetAppCacheService(resource_context), child_id,
      request_data.appcache_host_id, request_data.resource_type);

  if (deferred_request) {
    GlobalRequestID global_id(extra_info->GetChildID(),
                              extra_info->GetRequestID());
    pending_requests_[global_id] = request;
    request->FollowDeferredRedirect();
  } else {
    BeginRequestInternal(request);
  }
}
