void ResourceDispatcherHostImpl::BeginRequestInternal(
    scoped_ptr<net::URLRequest> request,
    scoped_ptr<ResourceHandler> handler) {
  DCHECK(!request->is_pending());
  ResourceRequestInfoImpl* info =
      ResourceRequestInfoImpl::ForRequest(request.get());

  if ((TimeTicks::Now() - last_user_gesture_time_) <
      TimeDelta::FromMilliseconds(kUserGestureWindowMs)) {
    request->set_load_flags(
        request->load_flags() | net::LOAD_MAYBE_USER_GESTURE);
  }

  info->set_memory_cost(CalculateApproximateMemoryCost(request.get()));
  int memory_cost = IncrementOutstandingRequestsMemoryCost(info->memory_cost(),
                                                           info->GetChildID());

  if (memory_cost > max_outstanding_requests_cost_per_process_) {
    request->CancelWithError(net::ERR_INSUFFICIENT_RESOURCES);

    if (!handler->OnResponseCompleted(info->GetRequestID(), request->status(),
                                      std::string())) {
      NOTREACHED();
    }

    IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                           info->GetChildID());

    handler.reset();
    return;
  }

  linked_ptr<ResourceLoader> loader(
      new ResourceLoader(request.Pass(), handler.Pass(), this));

  ProcessRouteIDs pair_id(info->GetChildID(), info->GetRouteID());
  BlockedLoadersMap::const_iterator iter = blocked_loaders_map_.find(pair_id);
  if (iter != blocked_loaders_map_.end()) {
    iter->second->push_back(loader);
    return;
  }

  StartLoading(info, loader);
}
