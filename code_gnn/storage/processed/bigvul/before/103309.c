bool GpuDataManager::GpuAccessAllowed() {
  uint32 mask = (~(preliminary_gpu_feature_flags_.flags())) |
                GpuFeatureFlags::kGpuFeatureAcceleratedCompositing;
  return (gpu_feature_flags_.flags() & mask) == 0;
}
