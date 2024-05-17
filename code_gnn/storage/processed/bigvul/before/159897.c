void TopSitesImpl::ResetThreadSafeCache() {
  base::AutoLock lock(lock_);
  MostVisitedURLList cached;
  ApplyBlacklist(cache_->top_sites(), &cached);
  thread_safe_cache_->SetTopSites(cached);
}
