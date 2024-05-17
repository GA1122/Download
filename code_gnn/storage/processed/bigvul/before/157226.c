bool IsBackgroundVideoPauseOptimizationEnabled() {
  return base::FeatureList::IsEnabled(kBackgroundVideoPauseOptimization);
}
