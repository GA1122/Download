void WebGLRenderingContextBase::DrawingBufferClientRestoreFramebufferBinding() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  RestoreCurrentFramebuffer();
}
