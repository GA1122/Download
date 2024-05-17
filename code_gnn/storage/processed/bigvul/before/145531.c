void ResourceDispatcherHostImpl::StartLoading(
    ResourceRequestInfoImpl* info,
    scoped_ptr<ResourceLoader> loader) {
  tracked_objects::ScopedTracker tracking_profile(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "456331 ResourceDispatcherHostImpl::StartLoading"));

  ResourceLoader* loader_ptr = loader.get();
  pending_loaders_[info->GetGlobalRequestID()] = std::move(loader);

  loader_ptr->StartRequest();
}
