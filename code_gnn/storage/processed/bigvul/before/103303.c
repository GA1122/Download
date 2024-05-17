void GpuDataManager::AppendRendererCommandLine(
    CommandLine* command_line) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(command_line);

  uint32 flags = gpu_feature_flags_.flags();
  if ((flags & GpuFeatureFlags::kGpuFeatureWebgl) &&
      !command_line->HasSwitch(switches::kDisableExperimentalWebGL))
    command_line->AppendSwitch(switches::kDisableExperimentalWebGL);
  if ((flags & GpuFeatureFlags::kGpuFeatureMultisampling) &&
      !command_line->HasSwitch(switches::kDisableGLMultisampling))
    command_line->AppendSwitch(switches::kDisableGLMultisampling);
  if (flags & GpuFeatureFlags::kGpuFeatureAcceleratedCompositing) {
    const char* switches[] = {
        switches::kDisableAcceleratedCompositing,
        switches::kDisableExperimentalWebGL,
        switches::kDisableAccelerated2dCanvas
    };
    const int switch_count = sizeof(switches) / sizeof(char*);
    for (int i = 0; i < switch_count; ++i) {
      if (!command_line->HasSwitch(switches[i]))
        command_line->AppendSwitch(switches[i]);
    }
  }
}
