bool RenderFrameHostImpl::IsFeatureEnabled(
    blink::mojom::FeaturePolicyFeature feature) {
  blink::mojom::PolicyValueType feature_type =
      feature_policy_->GetFeatureList().at(feature).second;
  return feature_policy_ &&
         feature_policy_->IsFeatureEnabledForOrigin(
             feature, GetLastCommittedOrigin(),
             blink::PolicyValue::CreateMaxPolicyValue(feature_type));
}
