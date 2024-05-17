void RenderFrameImpl::SetPendingNavigationParams(
    std::unique_ptr<NavigationParams> navigation_params) {
  pending_navigation_params_ = std::move(navigation_params);
}
