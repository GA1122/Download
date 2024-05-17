void ResourceDispatcherHostImpl::OnShutdown() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  is_shutdown_ = true;
  pending_loaders_.clear();

  update_load_states_timer_.reset();

  std::set<ProcessRouteIDs> ids;
  for (BlockedLoadersMap::const_iterator iter = blocked_loaders_map_.begin();
       iter != blocked_loaders_map_.end(); ++iter) {
    std::pair<std::set<ProcessRouteIDs>::iterator, bool> result =
        ids.insert(iter->first);
    DCHECK(result.second);
  }
  for (std::set<ProcessRouteIDs>::const_iterator iter = ids.begin();
       iter != ids.end(); ++iter) {
    CancelBlockedRequestsForRoute(iter->first, iter->second);
  }
}
