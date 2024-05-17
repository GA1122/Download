void GLES2DecoderImpl::ClearDeviceWindowRectangles() const {
  if (!feature_info_->feature_flags().ext_window_rectangles) {
    return;
  }
  api()->glWindowRectanglesEXTFn(GL_EXCLUSIVE_EXT, 0, nullptr);
}
