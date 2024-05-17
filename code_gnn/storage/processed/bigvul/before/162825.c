void ContextState::SetMaxWindowRectangles(size_t max) {
  window_rectangles_ = std::vector<GLint>(max * 4, 0);
}
