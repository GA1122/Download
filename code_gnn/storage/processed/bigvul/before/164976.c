ResourceDispatcherHostImpl::~ResourceDispatcherHostImpl() {
  DCHECK(outstanding_requests_stats_map_.empty());
  DCHECK(g_resource_dispatcher_host);
  DCHECK(main_thread_task_runner_->BelongsToCurrentThread());
  g_resource_dispatcher_host = nullptr;
}
