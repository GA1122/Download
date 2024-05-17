void RenderFrameHostImpl::DidSetFramePolicyHeaders(
    blink::WebSandboxFlags sandbox_flags,
    const blink::ParsedFeaturePolicy& parsed_header) {
  if (!is_active())
    return;
  frame_tree_node()->SetFeaturePolicyHeader(parsed_header);
  ResetFeaturePolicy();
  feature_policy_->SetHeaderPolicy(parsed_header);
  frame_tree_node()->UpdateActiveSandboxFlags(sandbox_flags);
  active_sandbox_flags_ = frame_tree_node()->active_sandbox_flags();
}
