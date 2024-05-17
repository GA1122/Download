IntSize WebGLRenderingContextBase::DrawingBufferSize() const {
  if (isContextLost())
    return IntSize(0, 0);
  return GetDrawingBuffer()->Size();
}
