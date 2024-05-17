void NavigationRequest::OnRendererAbortedNavigation() {
  frame_tree_node_->navigator()->CancelNavigation(frame_tree_node_, false);

}
