void SendOnForegroundedToGpuService() {
  content::GpuProcessHost::CallOnIO(
      content::GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED,
      false  ,
      base::BindRepeating([](content::GpuProcessHost* host) {
        if (host) {
          host->gpu_service()->OnForegrounded();
        }
      }));
}
