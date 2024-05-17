void RenderFrameHostImpl::OnSetHasReceivedUserGestureBeforeNavigation(
    bool value) {
  frame_tree_node_->OnSetHasReceivedUserGestureBeforeNavigation(value);
}
