void GpuProcessHost::DidInitialize(
    const gpu::GPUInfo& gpu_info,
    const gpu::GpuFeatureInfo& gpu_feature_info,
    const base::Optional<gpu::GPUInfo>& gpu_info_for_hardware_gpu,
    const base::Optional<gpu::GpuFeatureInfo>&
        gpu_feature_info_for_hardware_gpu) {
  UMA_HISTOGRAM_BOOLEAN("GPU.GPUProcessInitialized", true);
  status_ = SUCCESS;

  if (gpu_feature_info.IsWorkaroundEnabled(gpu::WAKE_UP_GPU_BEFORE_DRAWING)) {
    wake_up_gpu_before_drawing_ = true;
  }
  if (gpu_feature_info.IsWorkaroundEnabled(
          gpu::DONT_DISABLE_WEBGL_WHEN_COMPOSITOR_CONTEXT_LOST)) {
    dont_disable_webgl_when_compositor_context_lost_ = true;
  }

  GpuDataManagerImpl* gpu_data_manager = GpuDataManagerImpl::GetInstance();
  gpu_data_manager->UpdateGpuFeatureInfo(gpu_feature_info,
                                         gpu_feature_info_for_hardware_gpu);
  gpu_data_manager->UpdateGpuInfo(gpu_info, gpu_info_for_hardware_gpu);
  RunRequestGPUInfoCallbacks(gpu_data_manager->GetGPUInfo());
}
