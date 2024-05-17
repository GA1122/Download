NavigationHandleImpl* RenderFrameHostImpl::GetNavigationHandle() {
  return navigation_request() ? navigation_request()->navigation_handle()
                              : nullptr;
}
