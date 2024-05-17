void ResourceDispatcherHostImpl::BeginRequestInternal(
    std::unique_ptr<net::URLRequest> request,
    std::unique_ptr<ResourceHandler> handler,
    bool is_initiated_by_fetch_api,
    std::unique_ptr<network::ScopedThrottlingToken> throttling_token) {
  DCHECK(!request->is_pending());
  ResourceRequestInfoImpl* info =
      ResourceRequestInfoImpl::ForRequest(request.get());

  if ((info->GetPageTransition() & ui::PAGE_TRANSITION_FORWARD_BACK) &&
      IsResourceTypeFrame(info->GetResourceType()) &&
      request->url().SchemeIsHTTPOrHTTPS()) {
    LogBackForwardNavigationFlagsHistogram(request->load_flags());
  }

  if ((TimeTicks::Now() - last_user_gesture_time_) <
      TimeDelta::FromMilliseconds(kUserGestureWindowMs)) {
    request->SetLoadFlags(request->load_flags() | net::LOAD_MAYBE_USER_GESTURE);
  }

  info->set_memory_cost(CalculateApproximateMemoryCost(request.get()));

  bool exhausted = false;

  OustandingRequestsStats stats = IncrementOutstandingRequestsMemory(1, *info);
  if (stats.memory_cost > max_outstanding_requests_cost_per_process_)
    exhausted = true;

  if (info->keepalive()) {
    constexpr auto kMaxKeepaliveConnections =
        network::URLLoaderFactory::kMaxKeepaliveConnections;
    constexpr auto kMaxKeepaliveConnectionsPerProcess =
        network::URLLoaderFactory::kMaxKeepaliveConnectionsPerProcess;
    constexpr auto kMaxKeepaliveConnectionsPerProcessForFetchAPI = network::
        URLLoaderFactory::kMaxKeepaliveConnectionsPerProcessForFetchAPI;
    const auto& recorder = keepalive_statistics_recorder_;
    if (recorder.num_inflight_requests() >= kMaxKeepaliveConnections)
      exhausted = true;
    if (recorder.NumInflightRequestsPerProcess(info->GetChildID()) >=
        kMaxKeepaliveConnectionsPerProcess) {
      exhausted = true;
    }
    if (is_initiated_by_fetch_api &&
        recorder.NumInflightRequestsPerProcess(info->GetChildID()) >=
            kMaxKeepaliveConnectionsPerProcessForFetchAPI) {
      exhausted = true;
    }
  }

  if (exhausted) {
    request->CancelWithError(net::ERR_INSUFFICIENT_RESOURCES);

    bool was_resumed = false;
    handler->OnResponseCompleted(
        request->status(),
        std::make_unique<NullResourceController>(&was_resumed));
    DCHECK(was_resumed);

    IncrementOutstandingRequestsMemory(-1, *info);

    handler.reset();
    return;
  }

  ResourceContext* resource_context = info->GetContext();
  std::unique_ptr<ResourceLoader> loader(
      new ResourceLoader(std::move(request), std::move(handler), this,
                         resource_context, std::move(throttling_token)));

  GlobalFrameRoutingId id(info->GetChildID(), info->GetRenderFrameID());
  BlockedLoadersMap::const_iterator iter = blocked_loaders_map_.find(id);
  if (iter != blocked_loaders_map_.end()) {
    iter->second->push_back(std::move(loader));
    return;
  }

  StartLoading(info, std::move(loader));
}
