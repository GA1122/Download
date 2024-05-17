void NavigatorImpl::CancelNavigation(FrameTreeNode* frame_tree_node) {
  CHECK(IsBrowserSideNavigationEnabled());
  frame_tree_node->ResetNavigationRequest(false);
  if (frame_tree_node->IsMainFrame())
    navigation_data_.reset();
}
