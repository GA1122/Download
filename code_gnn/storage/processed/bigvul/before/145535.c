void ResourceDispatcherHostImpl::UpdateOutstandingRequestsStats(
    const ResourceRequestInfoImpl& info,
    const OustandingRequestsStats& stats) {
  if (stats.memory_cost == 0 && stats.num_requests == 0)
    outstanding_requests_stats_map_.erase(info.GetChildID());
  else
    outstanding_requests_stats_map_[info.GetChildID()] = stats;
}
