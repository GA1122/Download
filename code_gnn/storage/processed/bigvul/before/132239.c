void RenderFrameImpl::OnFailedNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool has_stale_copy_in_cache,
    int error_code) {
  bool is_reload = IsReload(common_params.navigation_type);
  bool is_history_navigation = request_params.page_state.IsValid();
  WebURLRequest::CachePolicy cache_policy =
      WebURLRequest::UseProtocolCachePolicy;
  RenderFrameImpl::PrepareRenderViewForNavigation(
      common_params.url, request_params, &is_reload, &cache_policy);

  GetContentClient()->SetActiveURL(common_params.url);

  pending_navigation_params_.reset(new NavigationParams(
      common_params, StartNavigationParams(), request_params));

  Send(new FrameHostMsg_DidStartProvisionalLoadForFrame(
      routing_id_, common_params.url));

  blink::WebURLError error =
      CreateWebURLError(common_params.url, has_stale_copy_in_cache, error_code);
  WebURLRequest failed_request = CreateURLRequestForNavigation(
      common_params, scoped_ptr<StreamOverrideParameters>(),
      frame_->isViewSourceModeEnabled());
  SendFailedProvisionalLoad(failed_request, error, frame_);

  if (!ShouldDisplayErrorPageForFailedLoad(error_code, common_params.url)) {
    Send(new FrameHostMsg_DidDropNavigation(routing_id_));
    return;
  }

  frame_->enableViewSourceMode(false);

  bool replace = is_reload || is_history_navigation ||
                 common_params.url == GetLoadingUrl();
  LoadNavigationErrorPage(failed_request, error, replace);
}
