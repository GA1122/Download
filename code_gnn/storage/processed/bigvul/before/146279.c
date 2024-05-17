void WebGLRenderingContextBase::DrawingBufferClientRestoreFramebufferBinding() {
  if (!ContextGL())
    return;
  RestoreCurrentFramebuffer();
}
