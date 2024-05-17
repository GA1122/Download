void ResourceDispatcherHostImpl::OnShutdown() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  is_shutdown_ = true;
  for (PendingRequestList::const_iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i) {
    transferred_navigations_.erase(i->first);
  }
  STLDeleteValues(&pending_requests_);
  update_load_states_timer_.reset();

  std::set<ProcessRouteIDs> ids;
  for (BlockedRequestMap::const_iterator iter = blocked_requests_map_.begin();
       iter != blocked_requests_map_.end(); ++iter) {
    std::pair<std::set<ProcessRouteIDs>::iterator, bool> result =
        ids.insert(iter->first);
    DCHECK(result.second);
  }
  for (std::set<ProcessRouteIDs>::const_iterator iter = ids.begin();
       iter != ids.end(); ++iter) {
    CancelBlockedRequestsForRoute(iter->first, iter->second);
  }
}
