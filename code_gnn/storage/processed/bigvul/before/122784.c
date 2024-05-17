void GpuProcessHost::CreateImage(
    gfx::PluginWindowHandle window,
    int client_id,
    int image_id,
    const CreateImageCallback& callback) {
  TRACE_EVENT0("gpu", "GpuProcessHostUIShim::CreateImage");

  DCHECK(CalledOnValidThread());

  if (Send(new GpuMsg_CreateImage(window, client_id, image_id))) {
    create_image_requests_.push(callback);
  } else {
    CreateImageError(callback, gfx::Size());
  }
}
