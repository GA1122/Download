void GLES2DecoderImpl::DoViewport(GLint x, GLint y, GLsizei width,
                                  GLsizei height) {
  state_.viewport_x = x;
  state_.viewport_y = y;
  state_.viewport_width = std::min(width, viewport_max_width_);
  state_.viewport_height = std::min(height, viewport_max_height_);
  glViewport(x, y, width, height);
}
