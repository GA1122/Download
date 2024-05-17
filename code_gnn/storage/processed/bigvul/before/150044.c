void LayerTreeHostImpl::SetMemoryPolicy(const ManagedMemoryPolicy& policy) {
  DCHECK(task_runner_provider_->IsImplThread());

  SetManagedMemoryPolicy(policy);

  if (!policy.bytes_limit_when_visible && resource_pool_ &&
      settings_.using_synchronous_renderer_compositor) {
    ReleaseTileResources();
    CleanUpTileManagerAndUIResources();

    NotifyAllTileTasksCompleted();

    CreateTileManagerResources();
    RecreateTileResources();
  }
}
