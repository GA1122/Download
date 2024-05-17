CGLContextObj WebPluginAcceleratedSurfaceProxy::context() {
  return surface_ ? surface_->context() : NULL;
}
