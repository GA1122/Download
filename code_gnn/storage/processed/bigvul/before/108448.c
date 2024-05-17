void ChromeRenderProcessObserver::ExecutePendingClearCache() {
  if (clear_cache_pending_) {
    clear_cache_pending_ = false;
    WebCache::clear();
  }
}
