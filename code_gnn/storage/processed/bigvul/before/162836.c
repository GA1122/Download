void ContextState::UpdateWindowRectangles() const {
  if (!feature_info_->feature_flags().ext_window_rectangles) {
    return;
  }

  if (current_draw_framebuffer_client_id == 0) {
    api()->glWindowRectanglesEXTFn(GL_EXCLUSIVE_EXT, 0, nullptr);
  } else {
    DCHECK_LE(static_cast<size_t>(num_window_rectangles),
              GetMaxWindowRectangles());
    const GLint* data =
        num_window_rectangles ? window_rectangles_.data() : nullptr;
    api()->glWindowRectanglesEXTFn(window_rectangles_mode,
                                   num_window_rectangles, data);
  }
}
