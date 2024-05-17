void ResourceDispatcherHostImpl::BeginRequestInternal(
    net::URLRequest* request) {
  DCHECK(!request->is_pending());
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  if ((TimeTicks::Now() - last_user_gesture_time_) <
      TimeDelta::FromMilliseconds(kUserGestureWindowMs)) {
    request->set_load_flags(
        request->load_flags() | net::LOAD_MAYBE_USER_GESTURE);
  }

  info->set_memory_cost(CalculateApproximateMemoryCost(request));
  int memory_cost = IncrementOutstandingRequestsMemoryCost(info->memory_cost(),
                                                           info->GetChildID());

  if (memory_cost > max_outstanding_requests_cost_per_process_) {
    request->CancelWithError(net::ERR_INSUFFICIENT_RESOURCES);

    GlobalRequestID global_id(info->GetChildID(), info->GetRequestID());
    pending_requests_[global_id] = request;
    ResponseCompleted(request);
    return;
  }

  std::pair<int, int> pair_id(info->GetChildID(), info->GetRouteID());
  BlockedRequestMap::const_iterator iter = blocked_requests_map_.find(pair_id);
  if (iter != blocked_requests_map_.end()) {
    iter->second->push_back(request);
    return;
  }

  GlobalRequestID global_id(info->GetChildID(), info->GetRequestID());
  pending_requests_[global_id] = request;

  bool defer_start = false;
  if (!info->resource_handler()->OnWillStart(
          info->GetRequestID(), request->url(),
          &defer_start)) {
    CancelRequestInternal(request, false);
    return;
  }

  if (!defer_start)
    StartRequest(request);
}
