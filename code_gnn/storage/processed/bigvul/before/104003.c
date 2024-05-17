void GLES2DecoderImpl::DoClearDepthf(GLclampf depth) {
  clear_depth_ = depth;
  glClearDepth(depth);
}
