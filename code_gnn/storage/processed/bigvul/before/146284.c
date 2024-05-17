void WebGLRenderingContextBase::DrawingBufferClientRestoreTexture2DBinding() {
  if (!ContextGL())
    return;
  RestoreCurrentTexture2D();
}
