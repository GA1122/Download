void ResourceDispatcherHostImpl::CancelRequestsForProcess(int child_id) {
  CancelRequestsForRoute(
      GlobalFrameRoutingId(child_id, MSG_ROUTING_NONE  ));
  const auto& map = keepalive_statistics_recorder_.per_process_records();
  if (map.find(child_id) != map.end())
    keepalive_statistics_recorder_.Unregister(child_id);
}
