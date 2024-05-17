void WebContentsImpl::DidFinishNavigation(NavigationHandle* navigation_handle) {
  for (auto& observer : observers_)
    observer.DidFinishNavigation(navigation_handle);

  if (navigation_handle->HasCommitted()) {
    BrowserAccessibilityManager* manager =
        static_cast<RenderFrameHostImpl*>(
            navigation_handle->GetRenderFrameHost())
            ->browser_accessibility_manager();
    if (manager) {
      if (navigation_handle->IsErrorPage()) {
        manager->NavigationFailed();
      } else {
        manager->NavigationSucceeded();
      }
    }
  }
}
