void Document::ApplyFeaturePolicy(const ParsedFeaturePolicy& declared_policy) {
  const FeaturePolicy::FeatureState* opener_feature_state = nullptr;
  if (frame_ && frame_->IsMainFrame() &&
      !frame_->Client()->GetOpenerFeatureState().empty()) {
    opener_feature_state = &frame_->Client()->GetOpenerFeatureState();
  }

  InitializeFeaturePolicy(declared_policy, GetOwnerContainerPolicy(),
                          GetParentFeaturePolicy(), opener_feature_state);

  is_vertical_scroll_enforced_ =
      frame_ && !frame_->IsMainFrame() &&
      RuntimeEnabledFeatures::ExperimentalProductivityFeaturesEnabled() &&
      !GetFeaturePolicy()->IsFeatureEnabled(
          mojom::FeaturePolicyFeature::kVerticalScroll);
}
