bool RenderFrameHostImpl::IsCurrent() {
  return this == frame_tree_node_->current_frame_host();
}
