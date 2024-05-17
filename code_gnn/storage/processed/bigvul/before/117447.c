bool WebGraphicsContext3DCommandBufferImpl::makeContextCurrent() {
  if (!MaybeInitializeGL())
    return false;
  return ContentGLContext::MakeCurrent(context_);
}
