void RenderFrameHostImpl::OnAbortNavigation() {
  TRACE_EVENT1("navigation", "RenderFrameHostImpl::OnAbortNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id());
  if (!is_active())
    return;
  frame_tree_node()->navigator()->OnAbortNavigation(frame_tree_node());
}
