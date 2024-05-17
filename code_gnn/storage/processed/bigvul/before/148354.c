void WebContentsImpl::DidRedirectNavigation(
    NavigationHandle* navigation_handle) {
  for (auto& observer : observers_)
    observer.DidRedirectNavigation(navigation_handle);

  if (navigation_handle->GetReloadType() != ReloadType::NONE) {
    NavigationHandleImpl* nhi =
        static_cast<NavigationHandleImpl*>(navigation_handle);
    BrowserAccessibilityManager* manager =
        nhi->frame_tree_node()
            ->current_frame_host()
            ->browser_accessibility_manager();
    if (manager)
      manager->UserIsReloading();
  }
}
