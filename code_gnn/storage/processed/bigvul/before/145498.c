bool ResourceDispatcherHostImpl::HasSufficientResourcesForRequest(
    net::URLRequest* request) {
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  OustandingRequestsStats stats = IncrementOutstandingRequestsCount(1, info);

  if (stats.num_requests > max_num_in_flight_requests_per_process_)
    return false;
  if (num_in_flight_requests_ > max_num_in_flight_requests_)
    return false;

  return true;
}
