bool WebGraphicsContext3DCommandBufferImpl::Initialize(
    const WebGraphicsContext3D::Attributes& attributes,
    bool bind_generates_resources,
    content::CauseForGpuLaunch cause) {
  TRACE_EVENT0("gpu", "WebGfxCtx3DCmdBfrImpl::initialize");

  attributes_ = attributes;
  bind_generates_resources_ = bind_generates_resources;
  DCHECK(!command_buffer_);

  if (!factory_)
    return false;

  if (attributes.preferDiscreteGPU)
    gpu_preference_ = gfx::PreferDiscreteGpu;

  bool retry = false;

  do {
    host_ = factory_->EstablishGpuChannelSync(cause);
    if (!host_)
      return false;
    DCHECK(host_->state() == GpuChannelHost::kConnected);

    if (!retry) {
      if (host_->WillGpuSwitchOccur(true, gpu_preference_)) {
        host_->ForciblyCloseChannel();
        ClearSharedContexts();
        retry = true;
      }
    } else {
      retry = false;
    }
  } while (retry);

  return true;
}
