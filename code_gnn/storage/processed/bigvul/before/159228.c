DownloadInterruptReason DownloadManagerImpl::BeginDownloadRequest(
    std::unique_ptr<net::URLRequest> url_request,
    const Referrer& referrer,
    ResourceContext* resource_context,
    bool is_content_initiated,
    int render_process_id,
    int render_view_route_id,
    int render_frame_route_id,
    bool do_not_prompt_for_login) {
  if (ResourceDispatcherHostImpl::Get()->is_shutdown())
    return DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN;

  ResourceDispatcherHostImpl::Get()->InitializeURLRequest(
      url_request.get(), referrer,
      true,   
      render_process_id, render_view_route_id, render_frame_route_id,
      PREVIEWS_OFF, resource_context);

  url_request->set_first_party_url_policy(
      net::URLRequest::UPDATE_FIRST_PARTY_URL_ON_REDIRECT);

  const GURL& url = url_request->original_url();

  if (!CanRequestURLFromRenderer(render_process_id, url))
    return DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST;

  const net::URLRequestContext* request_context = url_request->context();
  if (!request_context->job_factory()->IsHandledProtocol(url.scheme())) {
    DVLOG(1) << "Download request for unsupported protocol: "
             << url.possibly_invalid_spec();
    return DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST;
  }

  std::unique_ptr<ResourceHandler> handler(
      DownloadResourceHandler::Create(url_request.get()));

  ResourceDispatcherHostImpl::Get()->BeginURLRequest(
      std::move(url_request), std::move(handler), true,   
      is_content_initiated, do_not_prompt_for_login, resource_context);
  return DOWNLOAD_INTERRUPT_REASON_NONE;
}
