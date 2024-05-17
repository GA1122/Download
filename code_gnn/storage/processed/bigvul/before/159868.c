void TopSitesImpl::ApplyBlacklist(const MostVisitedURLList& urls,
                                  MostVisitedURLList* out) {
  const base::DictionaryValue* blacklist =
      pref_service_->GetDictionary(kMostVisitedURLsBlacklist);
  UMA_HISTOGRAM_BOOLEAN("TopSites.NumberOfApplyBlacklist", true);
  UMA_HISTOGRAM_COUNTS_100("TopSites.NumberOfBlacklistedItems",
      (blacklist ? blacklist->size() : 0));
  size_t num_non_forced_urls = 0;
  size_t num_forced_urls = 0;
  for (size_t i = 0; i < urls.size(); ++i) {
    if (!IsBlacklisted(urls[i].url)) {
      if (urls[i].last_forced_time.is_null()) {
        if (num_non_forced_urls >= kNonForcedTopSitesNumber)
          continue;
        num_non_forced_urls++;
      } else {
        if (num_forced_urls >= kForcedTopSitesNumber)
          continue;
        num_forced_urls++;
      }
      out->push_back(urls[i]);
    }
  }
}
