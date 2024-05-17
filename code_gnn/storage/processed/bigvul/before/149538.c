PreconnectManager* LoadingPredictor::preconnect_manager() {
  if (shutdown_ || !IsPreconnectFeatureEnabled())
    return nullptr;

  if (!preconnect_manager_) {
    preconnect_manager_ =
        std::make_unique<PreconnectManager>(GetWeakPtr(), profile_);
  }

  return preconnect_manager_.get();
}
