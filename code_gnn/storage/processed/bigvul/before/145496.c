ResourceDispatcherHostImpl::GetOutstandingRequestsStats(
    const ResourceRequestInfoImpl& info) {
  OutstandingRequestsStatsMap::iterator entry =
      outstanding_requests_stats_map_.find(info.GetChildID());
  OustandingRequestsStats stats = { 0, 0 };
  if (entry != outstanding_requests_stats_map_.end())
    stats = entry->second;
  return stats;
}
