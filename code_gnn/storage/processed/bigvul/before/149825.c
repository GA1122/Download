void LayerTreeHost::ResetGpuRasterizationTracking() {
  content_is_suitable_for_gpu_rasterization_ = true;
  gpu_rasterization_histogram_recorded_ = false;
}
