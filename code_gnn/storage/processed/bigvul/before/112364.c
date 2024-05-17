void ResourceDispatcherHostImpl::RemoveResourceContext(
    ResourceContext* context) {
  CHECK(ContainsKey(active_resource_contexts_, context));
  active_resource_contexts_.erase(context);
}
