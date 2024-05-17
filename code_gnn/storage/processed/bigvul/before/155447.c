bool ChromeContentBrowserClient::ShouldDisableSiteIsolation() {
  constexpr int kDefaultMemoryThresholdMb = 1024;

  if (base::FeatureList::IsEnabled(
          features::kSitePerProcessOnlyForHighMemoryClients)) {
    int memory_threshold_mb = base::GetFieldTrialParamByFeatureAsInt(
        features::kSitePerProcessOnlyForHighMemoryClients,
        features::kSitePerProcessOnlyForHighMemoryClientsParamName,
        kDefaultMemoryThresholdMb);
    return base::SysInfo::AmountOfPhysicalMemoryMB() <= memory_threshold_mb;
  }

#if defined(OS_ANDROID)
  if (base::SysInfo::AmountOfPhysicalMemoryMB() <= kDefaultMemoryThresholdMb) {
    return true;
  }
#endif

  return false;
}
