bool WebGLRenderingContextBase::PaintRenderingResultsToCanvas(
    SourceDrawingBuffer source_buffer) {
  if (isContextLost())
    return false;

  bool must_clear_now = ClearIfComposited() != kSkipped;
  if (!marked_canvas_dirty_ && !must_clear_now)
    return false;

  canvas()->ClearCopiedImage();
  marked_canvas_dirty_ = false;

  if (!canvas()->GetOrCreateCanvasResourceProvider())
    return false;

  if (!canvas()->ResourceProvider()->IsAccelerated())
    return false;

  ScopedTexture2DRestorer restorer(this);
  ScopedFramebufferRestorer fbo_restorer(this);

  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  if (!CopyRenderingResultsFromDrawingBuffer(canvas()->ResourceProvider(),
                                             source_buffer)) {
    NOTREACHED();
    return false;
  }

  return true;
}
