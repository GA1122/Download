bool Resource::MatchPreload(const FetchParameters& params,
                            base::SingleThreadTaskRunner*) {
  DCHECK(is_unused_preload_);
  is_unused_preload_ = false;

  if (preload_discovery_time_) {
    int time_since_discovery = static_cast<int>(
        1000 * (CurrentTimeTicksInSeconds() - preload_discovery_time_));
    DEFINE_STATIC_LOCAL(CustomCountHistogram, preload_discovery_histogram,
                        ("PreloadScanner.ReferenceTime", 0, 10000, 50));
    preload_discovery_histogram.Count(time_since_discovery);
  }
  return true;
}
