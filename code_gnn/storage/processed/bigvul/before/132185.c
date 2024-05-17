NavigationState* RenderFrameImpl::CreateNavigationStateFromPending() {
  if (!pending_navigation_params_->common_params.url.SchemeIs(
          url::kJavaScriptScheme)) {
    return NavigationStateImpl::CreateBrowserInitiated(
        pending_navigation_params_->common_params,
        pending_navigation_params_->start_params,
        pending_navigation_params_->request_params);
  }
  return NavigationStateImpl::CreateContentInitiated();
}
