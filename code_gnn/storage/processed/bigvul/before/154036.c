void GLES2DecoderImpl::DoLineWidth(GLfloat width) {
  api()->glLineWidthFn(
      std::min(std::max(width, line_width_range_[0]), line_width_range_[1]));
}
