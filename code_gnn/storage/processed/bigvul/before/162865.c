void GLManager::InitializeWithWorkarounds(
    const GLManager::Options& options,
    const GpuDriverBugWorkarounds& workarounds) {
  GpuDriverBugWorkarounds combined_workarounds(
      g_gpu_feature_info.enabled_gpu_driver_bug_workarounds);
  combined_workarounds.Append(workarounds);
  InitializeWithWorkaroundsImpl(options, combined_workarounds);
}
