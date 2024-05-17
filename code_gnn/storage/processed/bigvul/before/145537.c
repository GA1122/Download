ResourceDispatcherHostImpl::~ResourceDispatcherHostImpl() {
  DCHECK(outstanding_requests_stats_map_.empty());
  DCHECK(g_resource_dispatcher_host);
  g_resource_dispatcher_host = NULL;
}
