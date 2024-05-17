Value* GpuDataManager::GetFeatureStatus() {
  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  if (gpu_blacklist_.get())
    return gpu_blacklist_->GetFeatureStatus(GpuAccessAllowed(),
        browser_command_line.HasSwitch(
            switches::kDisableAcceleratedCompositing),
        browser_command_line.HasSwitch(
            switches::kDisableAccelerated2dCanvas),
        browser_command_line.HasSwitch(switches::kDisableExperimentalWebGL),
        browser_command_line.HasSwitch(switches::kDisableGLMultisampling));
  return NULL;
}
