void ResourceDispatcherHostImpl::BeginRequestInternal(
    scoped_ptr<net::URLRequest> request,
    scoped_ptr<ResourceHandler> handler) {
  DCHECK(!request->is_pending());
  ResourceRequestInfoImpl* info =
      ResourceRequestInfoImpl::ForRequest(request.get());

  if ((TimeTicks::Now() - last_user_gesture_time_) <
      TimeDelta::FromMilliseconds(kUserGestureWindowMs)) {
    request->SetLoadFlags(
        request->load_flags() | net::LOAD_MAYBE_USER_GESTURE);
  }

  info->set_memory_cost(CalculateApproximateMemoryCost(request.get()));

  OustandingRequestsStats stats = IncrementOutstandingRequestsMemory(1, *info);
  if (stats.memory_cost > max_outstanding_requests_cost_per_process_) {
    request->CancelWithError(net::ERR_INSUFFICIENT_RESOURCES);

    bool defer = false;
    handler->OnResponseCompleted(request->status(), std::string(), &defer);
    if (defer) {
      NOTREACHED();
    }

    IncrementOutstandingRequestsMemory(-1, *info);

    handler.reset();
    return;
  }

  scoped_ptr<ResourceLoader> loader(
      new ResourceLoader(std::move(request), std::move(handler), this));

  GlobalRoutingID id(info->GetGlobalRoutingID());
  BlockedLoadersMap::const_iterator iter = blocked_loaders_map_.find(id);
  if (iter != blocked_loaders_map_.end()) {
    iter->second->push_back(std::move(loader));
    return;
  }

  StartLoading(info, std::move(loader));
}
