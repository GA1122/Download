ResourceDispatcherHostImpl::~ResourceDispatcherHostImpl() {
  DCHECK(g_resource_dispatcher_host);
  g_resource_dispatcher_host = NULL;
}
