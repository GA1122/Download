bool TopSitesImpl::AddForcedURL(const GURL& url, const base::Time& time) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!loaded_) {
    return false;
  }

  size_t num_forced = cache_->GetNumForcedURLs();
  MostVisitedURLList new_list(cache_->top_sites());
  MostVisitedURL new_url;

  if (cache_->IsKnownURL(url)) {
    size_t index = cache_->GetURLIndex(url);
    if (new_list[index].last_forced_time.is_null())
      return false;

    new_url = new_list[index];
    new_list.erase(new_list.begin() + index);
    num_forced--;
  } else {
    new_url.url = url;
    new_url.redirects.push_back(url);
  }
  new_url.last_forced_time = time;
  MostVisitedURLList::iterator mid = new_list.begin() + num_forced;
  mid = new_list.insert(mid, new_url);
  std::inplace_merge(new_list.begin(), mid, mid + 1, ForcedURLComparator);
  SetTopSites(new_list, CALL_LOCATION_FROM_FORCED_URLS);
  return true;
}
