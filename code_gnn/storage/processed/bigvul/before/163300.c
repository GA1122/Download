bool IsMojoBlobsEnabled() {
  return base::FeatureList::IsEnabled(features::kMojoBlobs) ||
         base::FeatureList::IsEnabled(features::kNetworkService);
}
