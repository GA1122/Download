int WebGLRenderingContextBase::drawingBufferWidth() const {
  return isContextLost() ? 0 : GetDrawingBuffer()->Size().Width();
}
