bool IsSingleProcessMash() {
  return base::FeatureList::IsEnabled(features::kSingleProcessMash);
}
