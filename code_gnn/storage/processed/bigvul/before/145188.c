bool GpuProcessHost::ValidateHost(GpuProcessHost* host) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSingleProcess) ||
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kInProcessGPU) ||
      host->valid_) {
    return true;
  }

  host->ForceShutdown();
  return false;
}
