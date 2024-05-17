WebPluginAcceleratedSurfaceProxy::~WebPluginAcceleratedSurfaceProxy() {
  if (surface_) {
    surface_->Destroy();
    delete surface_;
    surface_ = NULL;
  }
}
