ThreadWatcherList::CrashDataThresholds::CrashDataThresholds(
    uint32 live_threads_threshold,
    uint32 unresponsive_threshold)
    : live_threads_threshold(live_threads_threshold),
      unresponsive_threshold(unresponsive_threshold) {
}
