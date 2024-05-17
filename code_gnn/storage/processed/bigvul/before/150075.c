void LayerTreeHostImpl::UpdateTreeResourcesForGpuRasterizationIfNeeded() {
  if (!need_update_gpu_rasterization_status_)
    return;
  if (!UpdateGpuRasterizationStatus())
    return;

  ReleaseTileResources();
  if (resource_pool_) {
    CleanUpTileManagerAndUIResources();
    CreateTileManagerResources();
  }
  RecreateTileResources();

  SetRequiresHighResToDraw();
}
