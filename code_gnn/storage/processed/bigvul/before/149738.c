void FeatureInfo::InitializeForTesting(
    const DisallowedFeatures& disallowed_features) {
  initialized_ = false;
  Initialize(CONTEXT_TYPE_OPENGLES2, false  ,
             disallowed_features);
}
