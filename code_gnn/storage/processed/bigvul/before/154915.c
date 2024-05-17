cc::Layer* WebGLRenderingContextBase::CcLayer() const {
  return isContextLost() ? nullptr : GetDrawingBuffer()->CcLayer();
}
