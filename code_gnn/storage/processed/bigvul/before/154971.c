WebGLRenderingContextBase::PaintRenderingResultsToDataArray(
    SourceDrawingBuffer source_buffer) {
  if (isContextLost())
    return nullptr;
  ClearIfComposited();
  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  ScopedFramebufferRestorer restorer(this);
  return GetDrawingBuffer()->PaintRenderingResultsToDataArray(source_buffer);
}
