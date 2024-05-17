NavigationState* RenderFrameImpl::CreateNavigationStateFromPending() {
  if (IsBrowserInitiated(pending_navigation_params_.get())) {
    return NavigationStateImpl::CreateBrowserInitiated(
        pending_navigation_params_->common_params,
        pending_navigation_params_->request_params);
  }
  return NavigationStateImpl::CreateContentInitiated();
}
