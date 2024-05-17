bool LayerTreeHostImpl::UpdateGpuRasterizationStatus() {
  if (!compositor_frame_sink_)
    return false;

  int requested_msaa_samples = RequestedMSAASampleCount();
  int max_msaa_samples = 0;
  ContextProvider* compositor_context_provider =
      compositor_frame_sink_->context_provider();
  bool gpu_rasterization_enabled = false;
  if (compositor_context_provider) {
    const auto& caps = compositor_context_provider->ContextCapabilities();
    gpu_rasterization_enabled = caps.gpu_rasterization;
    if (!caps.msaa_is_slow)
      max_msaa_samples = caps.max_samples;
  }

  bool use_gpu = false;
  bool use_msaa = false;
  bool using_msaa_for_complex_content =
      requested_msaa_samples > 0 && max_msaa_samples >= requested_msaa_samples;
  if (settings_.gpu_rasterization_forced) {
    use_gpu = true;
    gpu_rasterization_status_ = GpuRasterizationStatus::ON_FORCED;
    use_msaa = !content_is_suitable_for_gpu_rasterization_ &&
               using_msaa_for_complex_content;
    if (use_msaa) {
      gpu_rasterization_status_ = GpuRasterizationStatus::MSAA_CONTENT;
    }
  } else if (!gpu_rasterization_enabled) {
    gpu_rasterization_status_ = GpuRasterizationStatus::OFF_DEVICE;
  } else if (!has_gpu_rasterization_trigger_) {
    gpu_rasterization_status_ = GpuRasterizationStatus::OFF_VIEWPORT;
  } else if (content_is_suitable_for_gpu_rasterization_) {
    use_gpu = true;
    gpu_rasterization_status_ = GpuRasterizationStatus::ON;
  } else if (using_msaa_for_complex_content) {
    use_gpu = use_msaa = true;
    gpu_rasterization_status_ = GpuRasterizationStatus::MSAA_CONTENT;
  } else {
    gpu_rasterization_status_ = GpuRasterizationStatus::OFF_CONTENT;
  }

  if (use_gpu && !use_gpu_rasterization_) {
    if (!CanUseGpuRasterization()) {
      use_gpu = false;
      use_msaa = false;
      gpu_rasterization_status_ = GpuRasterizationStatus::OFF_DEVICE;
    }
  }

  if (use_gpu == use_gpu_rasterization_ && use_msaa == use_msaa_)
    return false;

  use_gpu_rasterization_ = use_gpu;
  use_msaa_ = use_msaa;
  return true;
}
