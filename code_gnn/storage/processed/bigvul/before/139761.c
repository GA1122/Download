ResourceFetcher::DeadResourceStatsRecorder::~DeadResourceStatsRecorder() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      CustomCountHistogram, hit_count_histogram,
      new CustomCountHistogram("WebCore.ResourceFetcher.HitCount", 0, 1000,
                               50));
  hit_count_histogram.Count(use_count_);
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      CustomCountHistogram, revalidate_count_histogram,
      new CustomCountHistogram("WebCore.ResourceFetcher.RevalidateCount", 0,
                               1000, 50));
  revalidate_count_histogram.Count(revalidate_count_);
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      CustomCountHistogram, load_count_histogram,
      new CustomCountHistogram("WebCore.ResourceFetcher.LoadCount", 0, 1000,
                               50));
  load_count_histogram.Count(load_count_);
}
