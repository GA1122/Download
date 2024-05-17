void NavigatorImpl::OnAbortNavigation(FrameTreeNode* frame_tree_node) {
  NavigationRequest* ongoing_navigation_request =
      frame_tree_node->navigation_request();
  if (!ongoing_navigation_request ||
      ongoing_navigation_request->browser_initiated()) {
    return;
  }

  CancelNavigation(frame_tree_node, false);
}
