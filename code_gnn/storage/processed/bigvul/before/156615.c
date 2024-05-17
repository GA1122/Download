void NavigatorImpl::CancelNavigation(FrameTreeNode* frame_tree_node,
                                     bool inform_renderer) {
  if (frame_tree_node->navigation_request() &&
      frame_tree_node->navigation_request()->navigation_handle()) {
    frame_tree_node->navigation_request()
        ->navigation_handle()
        ->set_net_error_code(net::ERR_ABORTED);
  }
  frame_tree_node->ResetNavigationRequest(false, inform_renderer);
  if (frame_tree_node->IsMainFrame())
    navigation_data_.reset();
}
