bool IsNetworkServiceRunningInProcess() {
  return base::FeatureList::IsEnabled(network::features::kNetworkService) &&
         (base::CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kSingleProcess) ||
          base::FeatureList::IsEnabled(features::kNetworkServiceInProcess));
}
