WebLayer* WebGLRenderingContextBase::PlatformLayer() const {
  return isContextLost() ? nullptr : GetDrawingBuffer()->PlatformLayer();
}
