void ContextState::DoLineWidth(GLfloat width) const {
  api()->glLineWidthFn(
      std::min(std::max(width, line_width_min_), line_width_max_));
}
