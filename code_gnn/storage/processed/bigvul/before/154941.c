void WebGLRenderingContextBase::DrawingBufferClientRestoreTexture2DBinding() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  RestoreCurrentTexture2D();
}
