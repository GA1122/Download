void TopSitesCache::CountForcedURLs() {
  num_forced_urls_ = 0;
  while (num_forced_urls_ < top_sites_.size()) {
    if (top_sites_[num_forced_urls_].last_forced_time.is_null())
      break;
    num_forced_urls_++;
  }
#if DCHECK_IS_ON()
  for (size_t i = num_forced_urls_; i < top_sites_.size(); ++i) {
    DCHECK(top_sites_[i].last_forced_time.is_null())
        << "All the forced URLs must appear before non-forced URLs.";
  }
#endif
}
