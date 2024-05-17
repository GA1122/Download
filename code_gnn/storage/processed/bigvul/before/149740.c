void FeatureInfo::InitializeForTesting(ContextType context_type) {
  initialized_ = false;
  Initialize(context_type, false  ,
             DisallowedFeatures());
}
