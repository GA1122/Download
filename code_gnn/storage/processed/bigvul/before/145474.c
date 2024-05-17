DownloadInterruptReason ResourceDispatcherHostImpl::BeginDownload(
    scoped_ptr<net::URLRequest> request,
    const Referrer& referrer,
    bool is_content_initiated,
    ResourceContext* context,
    int child_id,
    int render_view_route_id,
    int render_frame_route_id,
    bool prefer_cache,
    bool do_not_prompt_for_login,
    scoped_ptr<DownloadSaveInfo> save_info,
    uint32_t download_id,
    const DownloadStartedCallback& started_callback) {
  if (is_shutdown_)
    return CallbackAndReturn(started_callback,
                             DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN);

  const GURL& url = request->original_url();

  char url_buf[128];
  base::strlcpy(url_buf, url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);
  CHECK(ContainsKey(active_resource_contexts_, context));

  SetReferrerForRequest(request.get(), referrer);

  int extra_load_flags = net::LOAD_NORMAL;
  if (prefer_cache) {
    if (request->get_upload() != NULL)
      extra_load_flags |= net::LOAD_ONLY_FROM_CACHE;
    else
      extra_load_flags |= net::LOAD_PREFERRING_CACHE;
  } else {
    extra_load_flags |= net::LOAD_DISABLE_CACHE;
  }
  request->SetLoadFlags(request->load_flags() | extra_load_flags);

  request->set_first_party_url_policy(
      net::URLRequest::UPDATE_FIRST_PARTY_URL_ON_REDIRECT);

  if (!ChildProcessSecurityPolicyImpl::GetInstance()->
          CanRequestURL(child_id, url)) {
    VLOG(1) << "Denied unauthorized download request for "
            << url.possibly_invalid_spec();
    return CallbackAndReturn(started_callback,
                             DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST);
  }

  request_id_--;

  const net::URLRequestContext* request_context = context->GetRequestContext();
  if (!request_context->job_factory()->IsHandledURL(url)) {
    VLOG(1) << "Download request for unsupported protocol: "
            << url.possibly_invalid_spec();
    return CallbackAndReturn(started_callback,
                             DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST);
  }

  ResourceRequestInfoImpl* extra_info =
      CreateRequestInfo(child_id, render_view_route_id,
                        render_frame_route_id, true, context);
  extra_info->set_do_not_prompt_for_login(do_not_prompt_for_login);
  extra_info->AssociateWithRequest(request.get());   

  if (request->url().SchemeIs(url::kBlobScheme)) {
    ChromeBlobStorageContext* blob_context =
        GetChromeBlobStorageContextForResourceContext(context);
    storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
        request.get(),
        blob_context->context()->GetBlobDataFromPublicURL(request->url()));
  }

  scoped_ptr<ResourceHandler> handler(CreateResourceHandlerForDownload(
      request.get(), is_content_initiated, true, download_id,
      std::move(save_info), started_callback));

  BeginRequestInternal(std::move(request), std::move(handler));

  return DOWNLOAD_INTERRUPT_REASON_NONE;
}
