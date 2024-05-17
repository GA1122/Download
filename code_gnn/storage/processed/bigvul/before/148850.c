void RenderFrameHostManager::CancelPending() {
  CHECK(pending_render_frame_host_);
  TRACE_EVENT1("navigation", "RenderFrameHostManager::CancelPending",
               "FrameTreeNode id", frame_tree_node_->frame_tree_node_id());
  render_frame_host_->ClearPendingWebUI();

  bool pending_was_loading = pending_render_frame_host_->is_loading();
  DiscardUnusedFrame(UnsetPendingRenderFrameHost());
  if (pending_was_loading)
    frame_tree_node_->DidStopLoading();
}
