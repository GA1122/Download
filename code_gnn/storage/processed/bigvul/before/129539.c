ScopedRenderTo::~ScopedRenderTo() {
  if (framebuffer_)
    framebuffer_->OnDidRenderTo();
}
