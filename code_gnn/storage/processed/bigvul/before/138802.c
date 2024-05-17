void RenderFrameHostImpl::SetNavigationHandle(
    std::unique_ptr<NavigationHandleImpl> navigation_handle) {
  navigation_handle_ = std::move(navigation_handle);
}
