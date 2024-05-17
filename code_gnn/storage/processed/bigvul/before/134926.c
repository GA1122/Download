GetMemoryPressureThresholds() {
  using MemoryPressureMonitor = base::chromeos::MemoryPressureMonitor;

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          kMemoryPressureThresholds)) {
    const std::string group_name =
        base::FieldTrialList::FindFullName(kMemoryPressureExperimentName);
    if (group_name == kConservativeThreshold)
      return MemoryPressureMonitor::THRESHOLD_CONSERVATIVE;
    if (group_name == kAggressiveCacheDiscardThreshold)
      return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE_CACHE_DISCARD;
    if (group_name == kAggressiveTabDiscardThreshold)
      return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE_TAB_DISCARD;
    if (group_name == kAggressiveThreshold)
      return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE;
    return MemoryPressureMonitor::THRESHOLD_DEFAULT;
  }

  const std::string option =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          kMemoryPressureThresholds);
  if (option == kConservativeThreshold)
    return MemoryPressureMonitor::THRESHOLD_CONSERVATIVE;
  if (option == kAggressiveCacheDiscardThreshold)
    return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE_CACHE_DISCARD;
  if (option == kAggressiveTabDiscardThreshold)
    return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE_TAB_DISCARD;
  if (option == kAggressiveThreshold)
    return MemoryPressureMonitor::THRESHOLD_AGGRESSIVE;

  return MemoryPressureMonitor::THRESHOLD_DEFAULT;
}
