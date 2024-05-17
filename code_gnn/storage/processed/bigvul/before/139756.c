void ResourceFetcher::DeadResourceStatsRecorder::Update(
    RevalidationPolicy policy) {
  switch (policy) {
    case kReload:
    case kLoad:
      ++load_count_;
      return;
    case kRevalidate:
      ++revalidate_count_;
      return;
    case kUse:
      ++use_count_;
      return;
  }
}
