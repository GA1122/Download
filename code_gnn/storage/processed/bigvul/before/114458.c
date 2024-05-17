void RenderingHelper::Clear() {
  suppress_swap_to_display_ = false;
  width_ = 0;
  height_ = 0;
  texture_id_to_surface_index_.clear();
  message_loop_ = NULL;
  egl_display_ = EGL_NO_DISPLAY;
  egl_context_ = EGL_NO_CONTEXT;
  egl_surfaces_.clear();
  PlatformUnInitialize();
}
