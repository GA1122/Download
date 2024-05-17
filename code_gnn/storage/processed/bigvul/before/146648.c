bool DrawingBuffer::SetupRGBEmulationForBlitFramebuffer() {

  if (want_alpha_channel_ || anti_aliasing_mode_ != kNone)
    return false;

  if (!(ShouldUseChromiumImage() &&
        ContextProvider()->GetCapabilities().chromium_image_rgb_emulation))
    return false;

  if (!back_color_buffer_)
    return false;

  if (!back_color_buffer_->image_id)
    return false;


  GLuint rgb_texture = back_color_buffer_->rgb_workaround_texture_id;
  GLenum target = GC3D_TEXTURE_RECTANGLE_ARB;
  if (!rgb_texture) {
    gl_->GenTextures(1, &rgb_texture);
    gl_->BindTexture(target, rgb_texture);
    gl_->TexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl_->TexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_->TexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl_->TexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl_->BindTexImage2DWithInternalformatCHROMIUM(target, GL_RGB,
                                                  back_color_buffer_->image_id);
    back_color_buffer_->rgb_workaround_texture_id = rgb_texture;
  }

  gl_->FramebufferTexture2D(GL_DRAW_FRAMEBUFFER_ANGLE, GL_COLOR_ATTACHMENT0,
                            target, rgb_texture, 0);
  return true;
}
