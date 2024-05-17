GpuProcessHost* GpuProcessHost::Get(GpuProcessKind kind,
                                    content::CauseForGpuLaunch cause) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  GpuDataManagerImpl* gpu_data_manager = GpuDataManagerImpl::GetInstance();
  if (gpu_data_manager != NULL && !gpu_data_manager->GpuAccessAllowed())
    return NULL;

  if (g_gpu_process_hosts[kind] && HostIsValid(g_gpu_process_hosts[kind]))
    return g_gpu_process_hosts[kind];

  if (cause == content::CAUSE_FOR_GPU_LAUNCH_NO_LAUNCH)
    return NULL;

  int host_id;
  host_id = ++g_last_host_id;

  UMA_HISTOGRAM_ENUMERATION("GPU.GPUProcessLaunchCause",
                            cause,
                            content::CAUSE_FOR_GPU_LAUNCH_MAX_ENUM);

  GpuProcessHost* host = new GpuProcessHost(host_id, kind);
  if (host->Init())
    return host;

  delete host;
  return NULL;
}
