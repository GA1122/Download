size_t TopSitesCache::GetNumNonForcedURLs() const {
  return top_sites_.size() - num_forced_urls_;
}
