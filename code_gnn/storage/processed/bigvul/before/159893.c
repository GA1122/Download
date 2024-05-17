void TopSitesImpl::OnURLsDeleted(HistoryService* history_service,
                                 bool all_history,
                                 bool expired,
                                 const URLRows& deleted_rows,
                                 const std::set<GURL>& favicon_urls) {
  if (!loaded_)
    return;

  if (all_history) {
    SetTopSites(MostVisitedURLList(), CALL_LOCATION_FROM_OTHER_PLACES);
    backend_->ResetDatabase();
  } else {
    std::set<size_t> indices_to_delete;   
    for (const auto& row : deleted_rows) {
      if (cache_->IsKnownURL(row.url()))
        indices_to_delete.insert(cache_->GetURLIndex(row.url()));
    }

    if (indices_to_delete.empty())
      return;

    MostVisitedURLList new_top_sites(cache_->top_sites());
    for (std::set<size_t>::reverse_iterator i = indices_to_delete.rbegin();
         i != indices_to_delete.rend(); i++) {
      new_top_sites.erase(new_top_sites.begin() + *i);
    }
    SetTopSites(new_top_sites, CALL_LOCATION_FROM_OTHER_PLACES);
  }
  StartQueryForMostVisited();
}
