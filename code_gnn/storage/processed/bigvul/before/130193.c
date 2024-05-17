void RenderFrameHostImpl::OnSwappedOut(bool timed_out) {
  frame_tree_node_->render_manager()->SwappedOutFrame(this);
}
