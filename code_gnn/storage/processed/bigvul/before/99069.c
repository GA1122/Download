void ResourceDispatcherHost::BeginRequestInternal(URLRequest* request) {
  DCHECK(!request->is_pending());
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);

  info->set_memory_cost(CalculateApproximateMemoryCost(request));
  int memory_cost = IncrementOutstandingRequestsMemoryCost(info->memory_cost(),
                                                           info->child_id());

  if (memory_cost > max_outstanding_requests_cost_per_process_) {
    request->SimulateError(net::ERR_INSUFFICIENT_RESOURCES);

    GlobalRequestID global_id(info->child_id(), info->request_id());
    pending_requests_[global_id] = request;
    OnResponseCompleted(request);
    return;
  }

  std::pair<int, int> pair_id(info->child_id(), info->route_id());
  BlockedRequestMap::const_iterator iter = blocked_requests_map_.find(pair_id);
  if (iter != blocked_requests_map_.end()) {
    iter->second->push_back(request);
    return;
  }

  GlobalRequestID global_id(info->child_id(), info->request_id());
  pending_requests_[global_id] = request;

  bool defer_start = false;
  if (!info->resource_handler()->OnWillStart(
          info->request_id(), request->url(),
          &defer_start)) {
    CancelRequest(info->child_id(), info->request_id(), false);
    return;
  }

  if (!defer_start)
    InsertIntoResourceQueue(request, *info);
}
