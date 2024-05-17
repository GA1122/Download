GrGLInterface* WebGraphicsContext3DCommandBufferImpl::onCreateGrGLInterface() {
  return webkit_glue::CreateCommandBufferSkiaGLBinding();
}
