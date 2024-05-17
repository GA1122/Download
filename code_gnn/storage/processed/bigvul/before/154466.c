gfx::Vector2d GLES2DecoderPassthroughImpl::GetSurfaceDrawOffset() const {
  if (bound_draw_framebuffer_ != 0 || offscreen_) {
    return gfx::Vector2d();
  }
  return surface_->GetDrawOffset();
}
