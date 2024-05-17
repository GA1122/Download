void LayerTreeHostImpl::SetContentIsSuitableForGpuRasterization(bool flag) {
  if (content_is_suitable_for_gpu_rasterization_ != flag) {
    content_is_suitable_for_gpu_rasterization_ = flag;
    need_update_gpu_rasterization_status_ = true;
  }
}
