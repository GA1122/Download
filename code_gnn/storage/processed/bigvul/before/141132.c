bool Document::IsLazyLoadPolicyEnforced() const {
  return RuntimeEnabledFeatures::ExperimentalProductivityFeaturesEnabled() &&
         !GetFeaturePolicy()->IsFeatureEnabled(
             mojom::FeaturePolicyFeature::kLazyLoad);
}
