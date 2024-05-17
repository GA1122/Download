void BrowserGpuChannelHostFactory::CreateViewCommandBufferOnIO(
    CreateRequest* request,
    int32 surface_id,
    const GPUCreateCommandBufferConfig& init_params) {
  GpuProcessHost* host = GpuProcessHost::FromID(gpu_host_id_);
  if (!host) {
    request->event.Signal();
    return;
  }

  gfx::GLSurfaceHandle surface =
      GpuSurfaceTracker::Get()->GetSurfaceHandle(surface_id);

  host->CreateViewCommandBuffer(
      surface,
      surface_id,
      gpu_client_id_,
      init_params,
      base::Bind(&BrowserGpuChannelHostFactory::CommandBufferCreatedOnIO,
                 request));
}
