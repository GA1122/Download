FeatureInfo::FeatureInfo(
    const GpuDriverBugWorkarounds& gpu_driver_bug_workarounds,
    const GpuFeatureInfo& gpu_feature_info)
    : workarounds_(gpu_driver_bug_workarounds) {
  InitializeBasicState(base::CommandLine::InitializedForCurrentProcess()
                           ? base::CommandLine::ForCurrentProcess()
                           : nullptr);
  feature_flags_.chromium_raster_transport =
      gpu_feature_info.status_values[GPU_FEATURE_TYPE_OOP_RASTERIZATION] ==
      gpu::kGpuFeatureStatusEnabled;
  feature_flags_.android_surface_control =
      gpu_feature_info
          .status_values[GPU_FEATURE_TYPE_ANDROID_SURFACE_CONTROL] ==
      gpu::kGpuFeatureStatusEnabled;

#if defined(OS_CHROMEOS)
  feature_flags_.chromium_image_ycbcr_420v = base::Contains(
      gpu_feature_info.supported_buffer_formats_for_allocation_and_texturing,
      gfx::BufferFormat::YUV_420_BIPLANAR);
#elif defined(OS_MACOSX)
  feature_flags_.chromium_image_ycbcr_420v = true;
#endif

#if defined(OS_CHROMEOS)
  feature_flags_.chromium_image_ycbcr_p010 = base::Contains(
      gpu_feature_info.supported_buffer_formats_for_allocation_and_texturing,
      gfx::BufferFormat::P010);
#endif
}
