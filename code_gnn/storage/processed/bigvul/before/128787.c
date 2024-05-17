void GLSurfaceOzoneSurfacelessSurfaceImpl::BindFramebuffer() {
  ScopedFrameBufferBinder fb(fbo_);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                            textures_[current_surface_], 0);
}
