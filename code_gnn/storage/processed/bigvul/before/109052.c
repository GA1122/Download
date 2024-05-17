bool RenderViewImpl::WebWidgetHandlesCompositorScheduling() const {
  return !!RenderThreadImpl::current()->compositor_thread();
}
