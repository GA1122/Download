void PepperVideoRenderer3D::PaintIfNeeded() {
  bool need_repaint = next_picture_ || (force_repaint_ && current_picture_);
  if (paint_pending_ || !need_repaint)
    return;

  if (next_picture_)
    current_picture_ = next_picture_.Pass();

  force_repaint_ = false;
  latest_paint_started_time_ = base::TimeTicks::Now();

  const PP_VideoPicture& picture = current_picture_->picture();
  PP_Resource graphics_3d = graphics_.pp_resource();

  EnsureProgramForTexture(picture.texture_target);

  gles2_if_->UseProgram(graphics_3d, shader_program_);

  double scale_x = picture.visible_rect.size.width;
  double scale_y = picture.visible_rect.size.height;
  if (picture.texture_target != GL_TEXTURE_RECTANGLE_ARB) {
    scale_x /= picture.texture_size.width;
    scale_y /= picture.texture_size.height;
  }
  gles2_if_->Uniform2f(graphics_3d, shader_texcoord_scale_location_,
                       scale_x, scale_y);

  gles2_if_->Viewport(graphics_3d, 0, 0, view_size_.width(),
                      view_size_.height());

  gles2_if_->ActiveTexture(graphics_3d, GL_TEXTURE0);

  gles2_if_->BindTexture(graphics_3d, picture.texture_target,
                         picture.texture_id);

  gles2_if_->TexParameteri(graphics_3d, picture.texture_target,
                           GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  gles2_if_->DrawArrays(graphics_3d, GL_TRIANGLE_STRIP, 0, 4);

  CheckGLError();

  int32_t result = graphics_.SwapBuffers(
      callback_factory_.NewCallback(&PepperVideoRenderer3D::OnPaintDone));
  CHECK_EQ(result, PP_OK_COMPLETIONPENDING);
  paint_pending_ = true;
}
