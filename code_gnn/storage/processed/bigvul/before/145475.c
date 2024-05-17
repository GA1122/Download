void ResourceDispatcherHostImpl::BeginNavigationRequest(
    ResourceContext* resource_context,
    const NavigationRequestInfo& info,
    NavigationURLLoaderImplCore* loader,
    ServiceWorkerNavigationHandleCore* service_worker_handle_core) {
  CHECK(IsBrowserSideNavigationEnabled());

  ResourceType resource_type = info.is_main_frame ?
      RESOURCE_TYPE_MAIN_FRAME : RESOURCE_TYPE_SUB_FRAME;

  if (is_shutdown_ ||
      (delegate_ && !delegate_->ShouldBeginRequest(
          info.begin_params.method,
          info.common_params.url,
          resource_type,
          resource_context))) {
    loader->NotifyRequestFailed(false, net::ERR_ABORTED);
    return;
  }

  char url_buf[128];
  base::strlcpy(
      url_buf, info.common_params.url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);
  CHECK(ContainsKey(active_resource_contexts_, resource_context));

  const net::URLRequestContext* request_context =
      resource_context->GetRequestContext();

  int load_flags = info.begin_params.load_flags;
  load_flags |= net::LOAD_VERIFY_EV_CERT;
  if (info.is_main_frame)
    load_flags |= net::LOAD_MAIN_FRAME;


  DCHECK(!(load_flags & net::LOAD_IGNORE_LIMITS));

  scoped_ptr<net::URLRequest> new_request;
  new_request = request_context->CreateRequest(
      info.common_params.url, net::HIGHEST, nullptr);

  new_request->set_method(info.begin_params.method);
  new_request->set_first_party_for_cookies(
      info.first_party_for_cookies);
  new_request->set_initiator(info.request_initiator);
  if (info.is_main_frame) {
    new_request->set_first_party_url_policy(
        net::URLRequest::UPDATE_FIRST_PARTY_URL_ON_REDIRECT);
  }

  SetReferrerForRequest(new_request.get(), info.common_params.referrer);

  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(info.begin_params.headers);
  new_request->SetExtraRequestHeaders(headers);

  new_request->SetLoadFlags(load_flags);

  storage::BlobStorageContext* blob_context = GetBlobStorageContext(
      GetChromeBlobStorageContextForResourceContext(resource_context));

  if (info.request_body.get()) {
    AttachRequestBodyBlobDataHandles(
        info.request_body.get(),
        blob_context);
    new_request->set_upload(UploadDataStreamBuilder::Build(
        info.request_body.get(),
        blob_context,
        nullptr,   
        BrowserThread::GetMessageLoopProxyForThread(BrowserThread::FILE)
            .get()));
  }

  request_id_--;

  ResourceRequestInfoImpl* extra_info = new ResourceRequestInfoImpl(
      PROCESS_TYPE_BROWSER,
      -1,   
      -1,   
      info.frame_tree_node_id,
      -1,   
      request_id_,
      -1,   
      info.is_main_frame, info.parent_is_main_frame, resource_type,
      info.common_params.transition,
      false,
      false,   
      false,   
      info.common_params.allow_download, info.begin_params.has_user_gesture,
      true,    
      false,   
      false,   
      info.common_params.referrer.policy,
      blink::WebPageVisibilityStateVisible, resource_context,
      base::WeakPtr<ResourceMessageFilter>(),   
      false,   
      true,    
      IsUsingLoFi(info.common_params.lofi_state, delegate_, *new_request,
                  resource_context, info.is_main_frame),
      std::string());   
  extra_info->AssociateWithRequest(new_request.get());

  if (new_request->url().SchemeIs(url::kBlobScheme)) {
    storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
        new_request.get(),
        blob_context->GetBlobDataFromPublicURL(new_request->url()));
  }

  RequestContextFrameType frame_type =
      info.is_main_frame ? REQUEST_CONTEXT_FRAME_TYPE_TOP_LEVEL
                         : REQUEST_CONTEXT_FRAME_TYPE_NESTED;
  ServiceWorkerRequestHandler::InitializeForNavigation(
      new_request.get(), service_worker_handle_core, blob_context,
      info.begin_params.skip_service_worker, resource_type,
      info.begin_params.request_context_type, frame_type, info.request_body);


  scoped_ptr<ResourceHandler> handler(new NavigationResourceHandler(
      new_request.get(), loader));

  handler =
      AddStandardHandlers(new_request.get(), resource_type, resource_context,
                          nullptr,   
                          -1,        
                          -1,        
                          std::move(handler));

  BeginRequestInternal(std::move(new_request), std::move(handler));
}
