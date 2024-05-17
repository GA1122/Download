bool RenderFrameHostImpl::IsFeatureEnabled(
    blink::FeaturePolicyFeature feature) {
  return feature_policy_ && feature_policy_->IsFeatureEnabledForOrigin(
                                feature, GetLastCommittedOrigin());
}
