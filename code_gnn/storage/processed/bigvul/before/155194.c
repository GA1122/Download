NavigationHandleImpl* NavigatorImpl::GetNavigationHandleForFrameHost(
    RenderFrameHostImpl* render_frame_host) {
  return render_frame_host->navigation_handle();
}
