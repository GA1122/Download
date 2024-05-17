void FeatureInfo::InitializeForTesting() {
  initialized_ = false;
  Initialize(CONTEXT_TYPE_OPENGLES2, false  ,
             DisallowedFeatures());
}
