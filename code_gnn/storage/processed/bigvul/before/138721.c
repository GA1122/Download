void RenderFrameHostImpl::Navigate(
    const CommonNavigationParams& common_params,
    const StartNavigationParams& start_params,
    const RequestNavigationParams& request_params) {
  TRACE_EVENT0("navigation", "RenderFrameHostImpl::Navigate");
  DCHECK(!IsBrowserSideNavigationEnabled());

  UpdatePermissionsForNavigation(common_params, request_params);

  if (navigations_suspended_) {
    suspended_nav_params_.reset(
        new NavigationParams(common_params, start_params, request_params));
  } else {
    ResetWaitingState();
    SendNavigateMessage(common_params, start_params, request_params);
  }

  if (!common_params.url.SchemeIs(url::kJavaScriptScheme) &&
      (!navigation_handle_ || !navigation_handle_->is_transferring())) {
    OnDidStartLoading(true);
  }
}
