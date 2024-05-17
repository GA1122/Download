void WebContentsImpl::DidStartProvisionalLoad(
    RenderFrameHostImpl* render_frame_host,
    const GURL& validated_url,
    bool is_error_page,
    bool is_iframe_srcdoc) {
  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      DidStartProvisionalLoadForFrame(
          render_frame_host, validated_url, is_error_page, is_iframe_srcdoc));

  NavigationEntry* entry = controller_.GetVisibleEntry();
  if (entry && ui::PageTransitionCoreTypeIs(
          entry->GetTransitionType(), ui::PAGE_TRANSITION_RELOAD)) {
    FrameTreeNode* ftn = render_frame_host->frame_tree_node();
    BrowserAccessibilityManager* manager =
        ftn->current_frame_host()->browser_accessibility_manager();
    if (manager)
      manager->UserIsReloading();
  }
}
