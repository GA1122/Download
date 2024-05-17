void RenderFrameHostImpl::OnDidSetFeaturePolicyHeader(
    const ParsedFeaturePolicyHeader& parsed_header) {
  frame_tree_node()->SetFeaturePolicyHeader(parsed_header);
  ResetFeaturePolicy();
  feature_policy_->SetHeaderPolicy(parsed_header);
}
