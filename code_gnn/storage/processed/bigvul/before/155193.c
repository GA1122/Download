void NavigatorImpl::FailedNavigation(FrameTreeNode* frame_tree_node,
                                     bool has_stale_copy_in_cache,
                                     int error_code) {
  CHECK(IsBrowserSideNavigationEnabled());

  NavigationRequest* navigation_request = frame_tree_node->navigation_request();
  DCHECK(navigation_request);

  if (!IsRendererDebugURL(navigation_request->navigation_handle()->GetURL()))
    DiscardPendingEntryIfNeeded(navigation_request->navigation_handle());

  if (error_code == net::ERR_ABORTED) {
    frame_tree_node->ResetNavigationRequest(false);
    return;
  }

  RenderFrameHostImpl* render_frame_host =
      frame_tree_node->render_manager()->GetFrameHostForNavigation(
          *navigation_request);
  CheckWebUIRendererDoesNotDisplayNormalURL(
      render_frame_host, navigation_request->common_params().url);

  navigation_request->TransferNavigationHandleOwnership(render_frame_host);
  render_frame_host->navigation_handle()->ReadyToCommitNavigation(
      render_frame_host);
  render_frame_host->FailedNavigation(navigation_request->common_params(),
                                      navigation_request->request_params(),
                                      has_stale_copy_in_cache, error_code);
}
