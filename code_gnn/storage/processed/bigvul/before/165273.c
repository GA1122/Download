void NavigationRequest::OnRendererAbortedNavigation() {
  if (navigation_handle_->IsWaitingToCommit()) {
    render_frame_host_->NavigationRequestCancelled(this);
  } else {
    frame_tree_node_->navigator()->CancelNavigation(frame_tree_node_, false);
  }

}
