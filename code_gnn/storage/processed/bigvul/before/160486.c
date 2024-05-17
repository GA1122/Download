void RenderFrameHostImpl::SetNavigationRequest(
    std::unique_ptr<NavigationRequest> navigation_request) {
  navigation_request_ = std::move(navigation_request);
}
