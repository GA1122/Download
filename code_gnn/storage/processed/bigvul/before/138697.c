int RenderFrameHostImpl::GetProxyCount() {
  if (this != frame_tree_node_->current_frame_host())
    return 0;
  return frame_tree_node_->render_manager()->GetProxyCount();
}
