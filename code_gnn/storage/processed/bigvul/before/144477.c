void WebContentsImpl::DidFailProvisionalLoadWithError(
    RenderFrameHostImpl* render_frame_host,
    const FrameHostMsg_DidFailProvisionalLoadWithError_Params& params) {
  GURL validated_url(params.url);
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    DidFailProvisionalLoad(render_frame_host,
                                           validated_url,
                                           params.error_code,
                                           params.error_description,
                                           params.was_ignored_by_handler));

  FrameTreeNode* ftn = render_frame_host->frame_tree_node();
  BrowserAccessibilityManager* manager =
      ftn->current_frame_host()->browser_accessibility_manager();
  if (manager)
    manager->NavigationFailed();
}
