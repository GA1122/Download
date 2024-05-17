void GLManager::Initialize(const GLManager::Options& options) {
  GpuDriverBugWorkarounds platform_workarounds(
      g_gpu_feature_info.enabled_gpu_driver_bug_workarounds);
  InitializeWithWorkaroundsImpl(options, platform_workarounds);
}
