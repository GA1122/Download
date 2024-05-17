void GpuProcessHost::CreateViewCommandBuffer(
    const gfx::GLSurfaceHandle& compositing_surface,
    int surface_id,
    int client_id,
    const GPUCreateCommandBufferConfig& init_params,
    const CreateCommandBufferCallback& callback) {
  DCHECK(CalledOnValidThread());

#if defined(TOOLKIT_GTK)
  linked_ptr<SurfaceRef> surface_ref;
  SurfaceRefMap::iterator it = surface_refs_.find(surface_id);
  if (it != surface_refs_.end())
    surface_ref = (*it).second;
  else
    surface_ref.reset(new SurfaceRef(compositing_surface.handle));
#endif   

  if (!compositing_surface.is_null() &&
      Send(new GpuMsg_CreateViewCommandBuffer(
          compositing_surface, surface_id, client_id, init_params))) {
    create_command_buffer_requests_.push(callback);
#if defined(TOOLKIT_GTK)
    surface_refs_.insert(std::make_pair(surface_id, surface_ref));
#endif
  } else {
    CreateCommandBufferError(callback, MSG_ROUTING_NONE);
  }
}
