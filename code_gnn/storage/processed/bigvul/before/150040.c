void LayerTreeHostImpl::SetHasGpuRasterizationTrigger(bool flag) {
  if (has_gpu_rasterization_trigger_ != flag) {
    has_gpu_rasterization_trigger_ = flag;
    need_update_gpu_rasterization_status_ = true;
  }
}
