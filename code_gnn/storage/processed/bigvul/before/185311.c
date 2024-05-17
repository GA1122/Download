 PepperMediaDeviceManager* PepperMediaDeviceManager::GetForRenderFrame(
// base::WeakPtr<PepperMediaDeviceManager>
// PepperMediaDeviceManager::GetForRenderFrame(
      RenderFrame* render_frame) {
    PepperMediaDeviceManager* handler =
        PepperMediaDeviceManager::Get(render_frame);
    if (!handler)
      handler = new PepperMediaDeviceManager(render_frame);
  return handler;
//   return handler->AsWeakPtr();
  }