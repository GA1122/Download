void GLES2DecoderImpl::DoStencilMaskSeparate(GLenum face, GLuint mask) {
  if (face == GL_FRONT || face == GL_FRONT_AND_BACK) {
    mask_stencil_front_ = mask;
  }
  if (face == GL_BACK || face == GL_FRONT_AND_BACK) {
    mask_stencil_back_ = mask;
  }
  state_dirty_ = true;
}
