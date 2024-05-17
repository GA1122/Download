void TopSitesImpl::DiffMostVisited(const MostVisitedURLList& old_list,
                                   const MostVisitedURLList& new_list,
                                   TopSitesDelta* delta) {

  std::map<GURL, size_t> all_old_urls;
  size_t num_old_forced = 0;
  for (size_t i = 0; i < old_list.size(); i++) {
    if (!old_list[i].last_forced_time.is_null())
      num_old_forced++;
    DCHECK(old_list[i].last_forced_time.is_null() || i < num_old_forced)
        << "Forced URLs must all appear before non-forced URLs.";
    all_old_urls[old_list[i].url] = i;
  }

  const size_t kAlreadyFoundMarker = static_cast<size_t>(-1);
  int rank = -1;   
  for (size_t i = 0; i < new_list.size(); i++) {
    if (new_list[i].last_forced_time.is_null())
      rank++;
    DCHECK(new_list[i].last_forced_time.is_null() == (rank != -1))
        << "Forced URLs must all appear before non-forced URLs.";
    std::map<GURL, size_t>::iterator found = all_old_urls.find(new_list[i].url);
    if (found == all_old_urls.end()) {
      MostVisitedURLWithRank added;
      added.url = new_list[i];
      added.rank = rank;
      delta->added.push_back(added);
    } else {
      DCHECK(found->second != kAlreadyFoundMarker)
          << "Same URL appears twice in the new list.";
      int old_rank = found->second >= num_old_forced ?
          found->second - num_old_forced : -1;
      if (old_rank != rank ||
          old_list[found->second].last_forced_time !=
              new_list[i].last_forced_time) {
        MostVisitedURLWithRank moved;
        moved.url = new_list[i];
        moved.rank = rank;
        delta->moved.push_back(moved);
      }
      found->second = kAlreadyFoundMarker;
    }
  }

  for (const std::pair<GURL, size_t>& old_url : all_old_urls) {
    if (old_url.second != kAlreadyFoundMarker)
      delta->deleted.push_back(old_list[old_url.second]);
  }
}
