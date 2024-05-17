void GLES2DecoderImpl::DoStencilMask(GLuint mask) {
  mask_stencil_front_ = mask;
  mask_stencil_back_ = mask;
  state_dirty_ = true;
}
