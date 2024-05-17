 PepperMediaDeviceManager::PepperMediaDeviceManager(RenderFrame* render_frame)
    : RenderFrameObserver(render_frame),
      RenderFrameObserverTracker<PepperMediaDeviceManager>(render_frame),
      next_id_(1) {}
