bool ChromeContentBrowserClient::ShouldEnableStrictSiteIsolation() {
  if (base::FeatureList::IsEnabled(
          features::kSitePerProcessOnlyForHighMemoryClients)) {
    constexpr int kDefaultMemoryThresholdMb = 1024;
    int memory_threshold_mb = base::GetFieldTrialParamByFeatureAsInt(
        features::kSitePerProcessOnlyForHighMemoryClients,
        features::kSitePerProcessOnlyForHighMemoryClientsParamName,
        kDefaultMemoryThresholdMb);
    if (base::SysInfo::AmountOfPhysicalMemoryMB() <= memory_threshold_mb)
      return false;
  }

  return base::FeatureList::IsEnabled(features::kSitePerProcess);
}
