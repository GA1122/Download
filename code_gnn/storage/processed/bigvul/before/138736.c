void RenderFrameHostImpl::OnCreateChildFrame(
    int new_routing_id,
    blink::WebTreeScopeType scope,
    const std::string& frame_name,
    const std::string& frame_unique_name,
    blink::WebSandboxFlags sandbox_flags,
    const ParsedFeaturePolicyHeader& container_policy,
    const FrameOwnerProperties& frame_owner_properties) {
  DCHECK(!frame_unique_name.empty());

  if (!is_active() || frame_tree_node_->current_frame_host() != this ||
      !render_frame_created_)
    return;

  frame_tree_->AddFrame(frame_tree_node_, GetProcess()->GetID(), new_routing_id,
                        scope, frame_name, frame_unique_name, sandbox_flags,
                        container_policy, frame_owner_properties);
}
