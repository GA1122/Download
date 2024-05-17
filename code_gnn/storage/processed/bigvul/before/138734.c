void RenderFrameHostImpl::OnCancelInitialHistoryLoad() {
  if (GetParent() && !frame_tree_node_->has_committed_real_load() &&
      frame_tree_node_->render_manager()->pending_frame_host()) {
    frame_tree_node_->render_manager()->CancelPendingIfNecessary(
        frame_tree_node_->render_manager()->pending_frame_host());
  }
}
