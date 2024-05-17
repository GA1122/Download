bool RenderFrameHostImpl::IsFeatureEnabled(
    blink::WebFeaturePolicyFeature feature) {
  return feature_policy_ && feature_policy_->IsFeatureEnabledForOrigin(
                                feature, GetLastCommittedOrigin());
}
