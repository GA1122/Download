void ResourceDispatcherHostImpl::AddResourceContext(ResourceContext* context) {
  active_resource_contexts_.insert(context);
}
