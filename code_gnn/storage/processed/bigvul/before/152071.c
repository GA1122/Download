void RenderFrameHostImpl::ResetNavigationsForPendingDeletion() {
  for (auto& child : children_)
    child->current_frame_host()->ResetNavigationsForPendingDeletion();
  ResetNavigationRequests();
  frame_tree_node_->ResetNavigationRequest(false, false);
  frame_tree_node_->render_manager()->CleanUpNavigation();
}
