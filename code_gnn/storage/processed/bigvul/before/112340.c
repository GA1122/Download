int ResourceDispatcherHostImpl::IncrementOutstandingRequestsMemoryCost(
    int cost,
    int child_id) {
  OutstandingRequestsMemoryCostMap::iterator prev_entry =
      outstanding_requests_memory_cost_map_.find(child_id);
  int new_cost = 0;
  if (prev_entry != outstanding_requests_memory_cost_map_.end())
    new_cost = prev_entry->second;

  new_cost += cost;
  CHECK(new_cost >= 0);
  if (new_cost == 0)
    outstanding_requests_memory_cost_map_.erase(child_id);
  else
    outstanding_requests_memory_cost_map_[child_id] = new_cost;

  return new_cost;
}
