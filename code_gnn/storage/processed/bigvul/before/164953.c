void ResourceDispatcherHostImpl::OnShutdown() {
  DCHECK(io_thread_task_runner_->BelongsToCurrentThread());

  is_shutdown_ = true;

  weak_factory_on_io_.InvalidateWeakPtrs();

  pending_loaders_.clear();

  update_load_info_timer_.reset();

  std::set<GlobalFrameRoutingId> ids;
  for (const auto& blocked_loaders : blocked_loaders_map_) {
    std::pair<std::set<GlobalFrameRoutingId>::iterator, bool> result =
        ids.insert(blocked_loaders.first);
    DCHECK(result.second);
  }
  for (const auto& routing_id : ids) {
    CancelBlockedRequestsForRoute(routing_id);
  }

  scheduler_.reset();
}
