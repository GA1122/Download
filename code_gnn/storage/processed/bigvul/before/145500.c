ResourceDispatcherHostImpl::IncrementOutstandingRequestsCount(
    int count,
    ResourceRequestInfoImpl* info) {
  DCHECK_EQ(1, abs(count));
  num_in_flight_requests_ += count;

  DCHECK_NE(info->counted_as_in_flight_request(), count > 0);
  info->set_counted_as_in_flight_request(count > 0);

  OustandingRequestsStats stats = GetOutstandingRequestsStats(*info);
  stats.num_requests += count;
  DCHECK_GE(stats.num_requests, 0);
  UpdateOutstandingRequestsStats(*info, stats);

  return stats;
}
