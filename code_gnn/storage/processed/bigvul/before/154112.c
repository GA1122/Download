void GLES2DecoderImpl::DoWindowRectanglesEXT(GLenum mode,
                                             GLsizei n,
                                             const volatile GLint* box) {
  std::vector<GLint> box_copy(box, box + (n * 4));
  if (static_cast<size_t>(n) > state_.GetMaxWindowRectangles()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glWindowRectanglesEXT",
                       "count > GL_MAX_WINDOW_RECTANGLES_EXT");
    return;
  }
  for (int i = 0; i < n; ++i) {
    int boxindex = i * 4;
    if (box_copy[boxindex + 2] < 0 || box_copy[boxindex + 3] < 0) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glWindowRectanglesEXT",
                         "negative box width or height");
      return;
    }
  }
  state_.SetWindowRectangles(mode, n, box_copy.data());
  state_.UpdateWindowRectangles();
}
