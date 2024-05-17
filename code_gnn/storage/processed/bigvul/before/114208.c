void GpuMessageFilter::OnCreateViewCommandBuffer(
    int32 surface_id,
    const GPUCreateCommandBufferConfig& init_params,
    IPC::Message* reply) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  GpuSurfaceTracker* surface_tracker = GpuSurfaceTracker::Get();
  gfx::GLSurfaceHandle compositing_surface;

  int renderer_id = 0;
  int render_widget_id = 0;
  bool result = surface_tracker->GetRenderWidgetIDForSurface(
      surface_id, &renderer_id, &render_widget_id);
  if (result && renderer_id == render_process_id_) {
    compositing_surface = surface_tracker->GetSurfaceHandle(surface_id);
  } else {
    DLOG(ERROR) << "Renderer " << render_process_id_
                << " tried to access a surface for renderer " << renderer_id;
  }

  if (compositing_surface.parent_gpu_process_id &&
      compositing_surface.parent_gpu_process_id != gpu_process_id_) {
    linked_ptr<CreateViewCommandBufferRequest> request(
        new CreateViewCommandBufferRequest(surface_id, init_params, reply));
    pending_requests_.push_back(request);
    return;
  }

  GpuProcessHost* host = GpuProcessHost::FromID(gpu_process_id_);
  if (!host || compositing_surface.is_null()) {
    reply->set_reply_error();
    Send(reply);
    return;
  }

  host->CreateViewCommandBuffer(
      compositing_surface,
      surface_id,
      render_process_id_,
      init_params,
      base::Bind(&GpuMessageFilter::CreateCommandBufferCallback,
                 AsWeakPtr(),
                 reply));
}
