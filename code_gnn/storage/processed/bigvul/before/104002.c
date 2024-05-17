void GLES2DecoderImpl::DoClearColor(
      GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
  clear_red_ = red;
  clear_green_ = green;
  clear_blue_ = blue;
  clear_alpha_ = alpha;
  glClearColor(red, green, blue, alpha);
}
