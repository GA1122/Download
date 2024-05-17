  explicit BrowserCompositorOutputSurface(
      WebGraphicsContext3DCommandBufferImpl* context,
      int surface_id,
      BrowserCompositorOutputSurfaceProxy* output_surface_proxy)
      : context3D_(context),
        surface_id_(surface_id),
        client_(NULL),
        output_surface_proxy_(output_surface_proxy) {
    DetachFromThread();
  }
