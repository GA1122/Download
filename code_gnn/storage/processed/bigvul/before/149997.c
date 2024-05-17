void LayerTreeHostImpl::ReclaimResources(
    const ReturnedResourceArray& resources) {
  if (!resource_provider_)
    return;

  resource_provider_->ReceiveReturnsFromParent(resources);

  if (resource_pool_) {
    if (resource_pool_->memory_usage_bytes()) {
      const size_t kMegabyte = 1024 * 1024;

      UMA_HISTOGRAM_MEMORY_MB(
          "Renderer4.ResourcePoolMemoryUsage",
          static_cast<int>(resource_pool_->memory_usage_bytes() / kMegabyte));
    }

    resource_pool_->CheckBusyResources();
    resource_pool_->ReduceResourceUsage();
  }

  if (!visible_)
    resource_provider_->FlushPendingDeletions();
}
