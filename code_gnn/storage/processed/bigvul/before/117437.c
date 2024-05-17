bool WebGraphicsContext3DCommandBufferImpl::Initialize(
    const WebGraphicsContext3D::Attributes& attributes) {
  DCHECK(!context_);
  TRACE_EVENT0("gpu", "WebGfxCtx3DCmdBfrImpl::initialize");
  GpuChannelHostFactory* factory = GpuChannelHostFactory::instance();
  if (!factory)
    return false;

  if (attributes.noExtensions || !attributes.canRecoverFromContextLoss)
    gpu_preference_ = gfx::PreferDiscreteGpu;

  bool retry = false;

  do {
    host_ = factory->EstablishGpuChannelSync(
        content::
        CAUSE_FOR_GPU_LAUNCH_WEBGRAPHICSCONTEXT3DCOMMANDBUFFERIMPL_INITIALIZE);
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

  const content::GPUInfo& gpu_info = host_->gpu_info();
  UMA_HISTOGRAM_ENUMERATION(
      "GPU.WebGraphicsContext3D_Init_CanLoseContext",
      attributes.canRecoverFromContextLoss * 2 + gpu_info.can_lose_context,
      4);
  if (attributes.canRecoverFromContextLoss == false) {
    if (gpu_info.can_lose_context)
      return false;
  }

  attributes_ = attributes;
  return true;
}
