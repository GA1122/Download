void WebContentsImpl::DidCommitProvisionalLoad(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    ui::PageTransition transition_type) {
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    DidCommitProvisionalLoadForFrame(
                        render_frame_host, url, transition_type));

  BrowserAccessibilityManager* manager =
      render_frame_host->browser_accessibility_manager();
  if (manager)
    manager->NavigationSucceeded();
}
