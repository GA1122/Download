bool RenderFrameHostImpl::IsFeatureEnabled(
    blink::mojom::FeaturePolicyFeature feature,
    blink::PolicyValue threshold_value) {
  return feature_policy_ &&
         feature_policy_->IsFeatureEnabledForOrigin(
             feature, GetLastCommittedOrigin(), threshold_value);
}
