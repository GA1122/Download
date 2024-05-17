void RenderFrameHostImpl::ResetFeaturePolicy() {
  RenderFrameHostImpl* parent_frame_host = GetParent();
  if (!parent_frame_host && !frame_tree_node_->current_replication_state()
                                 .opener_feature_state.empty()) {
    DCHECK(base::FeatureList::IsEnabled(features::kFeaturePolicyForSandbox));
    feature_policy_ = blink::FeaturePolicy::CreateWithOpenerPolicy(
        frame_tree_node_->current_replication_state().opener_feature_state,
        last_committed_origin_);
    return;
  }
  const blink::FeaturePolicy* parent_policy =
      parent_frame_host ? parent_frame_host->feature_policy() : nullptr;
  blink::ParsedFeaturePolicy container_policy =
      frame_tree_node()->effective_frame_policy().container_policy;
  feature_policy_ = blink::FeaturePolicy::CreateFromParentPolicy(
      parent_policy, container_policy, last_committed_origin_);
}
