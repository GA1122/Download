bool GpuProcessHost::HostIsValid(GpuProcessHost* host) {
  if (!host)
    return false;

  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kSingleProcess) ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessGPU) ||
      (host->valid_ &&
       (host->software_rendering() ||
        !GpuDataManagerImpl::GetInstance()->ShouldUseSoftwareRendering()))) {
    return true;
  }

  host->ForceShutdown();
  return false;
}
