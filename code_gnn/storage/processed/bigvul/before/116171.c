ResourceDispatcherHostImpl::~ResourceDispatcherHostImpl() {
  DCHECK(g_resource_dispatcher_host);
  g_resource_dispatcher_host = NULL;
  AsyncResourceHandler::GlobalCleanup();
  for (PendingRequestList::const_iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i) {
    transferred_navigations_.erase(i->first);
  }
  STLDeleteValues(&pending_requests_);
  DCHECK(transferred_navigations_.empty());
}
