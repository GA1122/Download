void GLES2DecoderImpl::DoViewport(GLint x, GLint y, GLsizei width,
                                  GLsizei height) {
  viewport_x_ = x;
  viewport_y_ = y;
  viewport_width_ = std::min(width, viewport_max_width_);
  viewport_height_ = std::min(height, viewport_max_height_);
  glViewport(x, y, width, height);
}
