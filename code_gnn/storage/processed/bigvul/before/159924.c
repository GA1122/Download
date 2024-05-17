  void EmptyThreadSafeCache() {
    base::AutoLock lock(top_sites()->lock_);
    MostVisitedURLList empty;
    top_sites()->thread_safe_cache_->SetTopSites(empty);
  }
