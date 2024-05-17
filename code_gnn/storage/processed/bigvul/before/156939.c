void NavigationRequest::OnResponseStarted(
    const scoped_refptr<network::ResourceResponse>& response,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<NavigationData> navigation_data,
    const GlobalRequestID& request_id,
    bool is_download,
    bool is_stream,
    base::Optional<SubresourceLoaderParams> subresource_loader_params) {
  DCHECK(state_ == STARTED);
  DCHECK(response);
  TRACE_EVENT_ASYNC_STEP_INTO0("navigation", "NavigationRequest", this,
                               "OnResponseStarted");
  state_ = RESPONSE_STARTED;

  response_should_be_rendered_ =
      !is_download && (!response->head.headers.get() ||
                       (response->head.headers->response_code() != 204 &&
                        response->head.headers->response_code() != 205));

  if (!response_should_be_rendered_) {
    navigation_handle_->set_net_error_code(net::ERR_ABORTED);
    net_error_ = net::ERR_ABORTED;
  }

  request_params_.service_worker_provider_id =
      navigation_handle_->service_worker_handle()
          ? navigation_handle_->service_worker_handle()
                ->service_worker_provider_host_id()
          : kInvalidServiceWorkerProviderId;
  request_params_.appcache_host_id =
      navigation_handle_->appcache_handle()
          ? navigation_handle_->appcache_handle()->appcache_host_id()
          : kAppCacheNoHostId;

  request_params_.was_activated = false;
  if (navigation_handle_->IsRendererInitiated() &&
      frame_tree_node_->has_received_user_gesture() &&
      ShouldPropagateUserActivation(
          frame_tree_node_->current_origin(),
          url::Origin::Create(navigation_handle_->GetURL()))) {
    request_params_.was_activated = true;
  } else if (((navigation_handle_->HasUserGesture() &&
               navigation_handle_->IsRendererInitiated()) ||
              navigation_handle_->WasStartedFromContextMenu()) &&
             ShouldPropagateUserActivation(
                 url::Origin::Create(navigation_handle_->GetReferrer().url),
                 url::Origin::Create(navigation_handle_->GetURL()))) {
    request_params_.was_activated = true;
  }

  common_params_.previews_state =
      static_cast<PreviewsState>(response->head.previews_state);

  RenderFrameHostImpl* render_frame_host = nullptr;
  if (response_should_be_rendered_) {
    render_frame_host =
        frame_tree_node_->render_manager()->GetFrameHostForNavigation(*this);
    NavigatorImpl::CheckWebUIRendererDoesNotDisplayNormalURL(
        render_frame_host, common_params_.url);
  }
  DCHECK(render_frame_host || !response_should_be_rendered_);

  if (!browser_initiated_ && render_frame_host &&
      render_frame_host != frame_tree_node_->current_frame_host()) {
    common_params_.source_location.reset();

    if (!frame_tree_node_->navigator()->GetDelegate()->ShouldTransferNavigation(
            frame_tree_node_->IsMainFrame())) {
      navigation_handle_->set_net_error_code(net::ERR_ABORTED);
      frame_tree_node_->ResetNavigationRequest(false, true);
      return;
    }
  }

  if (navigation_data)
    navigation_handle_->set_navigation_data(std::move(navigation_data));

  response_ = response;
  url_loader_client_endpoints_ = std::move(url_loader_client_endpoints);
  ssl_info_ = response->head.ssl_info.has_value() ? *response->head.ssl_info
                                                  : net::SSLInfo();
  is_download_ = is_download;

  subresource_loader_params_ = std::move(subresource_loader_params);

  if (render_frame_host &&
      SiteInstanceImpl::ShouldAssignSiteForURL(common_params_.url)) {
    render_frame_host->GetProcess()->SetIsUsed();

    SiteInstanceImpl* instance = render_frame_host->GetSiteInstance();
    if (!instance->HasSite() &&
        SiteInstanceImpl::DoesSiteRequireDedicatedProcess(
            instance->GetBrowserContext(), common_params_.url)) {
      instance->SetSite(common_params_.url);
    }
  }

  RenderFrameDevToolsAgentHost::OnNavigationResponseReceived(*this, *response);

  if (is_download && (response->head.headers.get() &&
                      (response->head.headers->response_code() / 100 != 2))) {
    navigation_handle_->set_net_error_code(net::ERR_INVALID_RESPONSE);
    frame_tree_node_->ResetNavigationRequest(false, true);
    return;
  }

  navigation_handle_->WillProcessResponse(
      render_frame_host, response->head.headers.get(),
      response->head.connection_info, response->head.socket_address, ssl_info_,
      request_id, common_params_.should_replace_current_entry, is_download,
      is_stream,
      base::Bind(&NavigationRequest::OnWillProcessResponseChecksComplete,
                 base::Unretained(this)));
}
