void GpuProcessHost::ForceShutdown() {
  if (g_gpu_process_hosts[kind_] == this)
    g_gpu_process_hosts[kind_] = NULL;

  process_->ForceShutdown();
}
