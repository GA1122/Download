bool WebGLRenderingContextBase::PaintRenderingResultsToCanvas(
    SourceDrawingBuffer source_buffer) {
  if (isContextLost())
    return false;

  bool must_clear_now = ClearIfComposited() != kSkipped;
  if (!must_paint_to_canvas_ && !must_clear_now)
    return false;

  must_paint_to_canvas_ = false;

  if (Host()->ResourceProvider() &&
      Host()->ResourceProvider()->Size() != GetDrawingBuffer()->Size()) {
    Host()->DiscardResourceProvider();
  }

  if (!Host()->GetOrCreateCanvasResourceProvider(kPreferAcceleration))
    return false;

  ScopedTexture2DRestorer restorer(this);
  ScopedFramebufferRestorer fbo_restorer(this);

  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  if (!CopyRenderingResultsFromDrawingBuffer(Host()->ResourceProvider(),
                                             source_buffer)) {
    NOTREACHED();
    return false;
  }
  return true;
}
