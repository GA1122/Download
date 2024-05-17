void GLES2DecoderPassthroughImpl::ApplySurfaceDrawOffset() {
  if (offscreen_ || !surface_->SupportsDCLayers()) {
    return;
  }

  gfx::Vector2d framebuffer_offset = GetSurfaceDrawOffset();
  api()->glViewportFn(viewport_[0] + framebuffer_offset.x(),
                      viewport_[1] + framebuffer_offset.y(), viewport_[2],
                      viewport_[3]);
  api()->glScissorFn(scissor_[0] + framebuffer_offset.x(),
                     scissor_[1] + framebuffer_offset.y(), scissor_[2],
                     scissor_[3]);
}
