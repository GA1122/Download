size_t TopSitesImpl::MergeCachedForcedURLs(MostVisitedURLList* new_list) const {
  DCHECK(thread_checker_.CalledOnValidThread());
  std::set<GURL> all_new_urls;
  size_t num_forced = 0;
  for (size_t i = 0; i < new_list->size(); ++i) {
    for (size_t j = 0; j < (*new_list)[i].redirects.size(); j++)
      all_new_urls.insert((*new_list)[i].redirects[j]);

    if (!(*new_list)[i].last_forced_time.is_null())
      ++num_forced;
  }

  MostVisitedURLList filtered_forced_urls;
  for (size_t i = 0; i < cache_->GetNumForcedURLs(); ++i) {
    if (all_new_urls.find(cache_->top_sites()[i].url) == all_new_urls.end())
      filtered_forced_urls.push_back(cache_->top_sites()[i]);
  }
  num_forced += filtered_forced_urls.size();

  new_list->insert(new_list->begin(), filtered_forced_urls.begin(),
                   filtered_forced_urls.end());
  std::inplace_merge(
      new_list->begin(), new_list->begin() + filtered_forced_urls.size(),
      new_list->begin() + num_forced, ForcedURLComparator);

  if (num_forced > kForcedTopSitesNumber) {
    new_list->erase(new_list->begin(),
                    new_list->begin() + (num_forced - kForcedTopSitesNumber));
    num_forced = kForcedTopSitesNumber;
  }

  return num_forced;
}
