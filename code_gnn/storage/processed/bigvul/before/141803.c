void RegisterFileMetricsPreferences(PrefRegistrySimple* registry) {
  metrics::FileMetricsProvider::RegisterPrefs(registry, kBrowserMetricsName);

  metrics::FileMetricsProvider::RegisterPrefs(registry,
                                              kCrashpadHistogramAllocatorName);

#if defined(OS_WIN)
  metrics::FileMetricsProvider::RegisterPrefs(
      registry, installer::kSetupHistogramAllocatorName);

  metrics::FileMetricsProvider::RegisterPrefs(
      registry, notification_helper::kNotificationHelperHistogramAllocatorName);
#endif
}
