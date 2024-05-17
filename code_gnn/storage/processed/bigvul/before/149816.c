void LayerTreeHost::RecordGpuRasterizationHistogram(
    const LayerTreeHostImpl* host_impl) {
  if (gpu_rasterization_histogram_recorded_ || IsSingleThreaded())
    return;

  bool gpu_rasterization_enabled = false;
  if (host_impl->compositor_frame_sink()) {
    ContextProvider* compositor_context_provider =
        host_impl->compositor_frame_sink()->context_provider();
    if (compositor_context_provider) {
      gpu_rasterization_enabled =
          compositor_context_provider->ContextCapabilities().gpu_rasterization;
    }
  }

  UMA_HISTOGRAM_BOOLEAN("Renderer4.GpuRasterizationEnabled",
                        gpu_rasterization_enabled);
  if (gpu_rasterization_enabled) {
    UMA_HISTOGRAM_BOOLEAN("Renderer4.GpuRasterizationTriggered",
                          has_gpu_rasterization_trigger_);
    UMA_HISTOGRAM_BOOLEAN("Renderer4.GpuRasterizationSuitableContent",
                          content_is_suitable_for_gpu_rasterization_);
    UMA_HISTOGRAM_BOOLEAN("Renderer4.GpuRasterizationUsed",
                          (has_gpu_rasterization_trigger_ &&
                           content_is_suitable_for_gpu_rasterization_));
  }

  gpu_rasterization_histogram_recorded_ = true;
}
