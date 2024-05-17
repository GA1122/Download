void RenderFrameHostImpl::DidSetFramePolicyHeaders(
    blink::WebSandboxFlags sandbox_flags,
    const blink::ParsedFeaturePolicy& parsed_header) {
  if (!is_active())
    return;
  ResetFeaturePolicy();
  feature_policy_->SetHeaderPolicy(parsed_header);

  frame_tree_node()->UpdateFramePolicyHeaders(sandbox_flags, parsed_header);

  active_sandbox_flags_ = frame_tree_node()->active_sandbox_flags();
}
