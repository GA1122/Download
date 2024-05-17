int WebGLRenderingContextBase::drawingBufferHeight() const {
  return isContextLost() ? 0 : GetDrawingBuffer()->Size().Height();
}
