  GpuProcessTransportFactory()
      : ALLOW_THIS_IN_INITIALIZER_LIST(callback_factory_(this)) {
    output_surface_proxy_ = new BrowserCompositorOutputSurfaceProxy();
  }
