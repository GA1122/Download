void ResourceDispatcherHostImpl::AddResourceContext(ResourceContext* context) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  active_resource_contexts_.insert(context);
}
