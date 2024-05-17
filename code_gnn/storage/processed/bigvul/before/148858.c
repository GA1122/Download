void RenderFrameHostManager::CommitPendingWebUI() {
  TRACE_EVENT1("navigation", "RenderFrameHostManager::CommitPendingWebUI",
               "FrameTreeNode id", frame_tree_node_->frame_tree_node_id());
  DCHECK(render_frame_host_->pending_web_ui());

  bool will_focus_location_bar = delegate_->FocusLocationBarByDefault();

  render_frame_host_->CommitPendingWebUI();

  if (will_focus_location_bar)
    delegate_->SetFocusToLocationBar(false);
}
