  bool ClientExposedBackBufferHasAlpha() const {
    if (back_buffer_draw_buffer_ == GL_NONE)
      return false;
    if (offscreen_target_frame_buffer_.get()) {
      return offscreen_buffer_should_have_alpha_;
    }
    return (back_buffer_color_format_ == GL_RGBA ||
            back_buffer_color_format_ == GL_RGBA8);
  }
