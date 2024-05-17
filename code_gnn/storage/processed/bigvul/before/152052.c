void RenderFrameHostImpl::OnUpdateUserActivationState(
    blink::UserActivationUpdateType update_type) {
  if (!is_active())
    return;
  frame_tree_node_->UpdateUserActivationState(update_type);
}
