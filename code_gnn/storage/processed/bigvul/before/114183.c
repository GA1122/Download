void BrowserGpuChannelHostFactory::EstablishGpuChannelOnIO(
    EstablishRequest* request,
    CauseForGpuLaunch cause_for_gpu_launch) {
  GpuProcessHost* host = GpuProcessHost::FromID(gpu_host_id_);
  if (!host) {
    host = GpuProcessHost::Get(GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED,
                               cause_for_gpu_launch);
    if (!host) {
      request->event.Signal();
      return;
    }
    gpu_host_id_ = host->host_id();
  }

  host->EstablishGpuChannel(
      gpu_client_id_,
      true,
      base::Bind(&BrowserGpuChannelHostFactory::GpuChannelEstablishedOnIO,
                 request));
}
