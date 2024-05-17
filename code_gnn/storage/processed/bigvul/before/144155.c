bool IsAshInBrowserProcess() {
  return !base::FeatureList::IsEnabled(features::kMash);
}
