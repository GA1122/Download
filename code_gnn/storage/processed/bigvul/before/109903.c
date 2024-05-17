bool GpuProcessHost::HostIsValid(GpuProcessHost* host) {
  if (!host)
    return false;

  if (host->process_->disconnect_was_alive())
    return false;

  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kSingleProcess) ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessGPU) ||
      host->software_rendering() ||
      !GpuDataManagerImpl::GetInstance()->ShouldUseSoftwareRendering()) {
    return true;
  }

  host->ForceShutdown();
  return false;
}
