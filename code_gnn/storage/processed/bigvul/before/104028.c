void GLES2DecoderImpl::DoGetRenderbufferParameteriv(
    GLenum target, GLenum pname, GLint* params) {
  if (!bound_renderbuffer_) {
    SetGLError(GL_INVALID_OPERATION,
               "glGetRenderbufferParameteriv: no renderbuffer bound");
    return;
  }
  switch (pname) {
  case GL_RENDERBUFFER_INTERNAL_FORMAT:
    *params = bound_renderbuffer_->internal_format();
    break;
  case GL_RENDERBUFFER_WIDTH:
    *params = bound_renderbuffer_->width();
    break;
  case GL_RENDERBUFFER_HEIGHT:
    *params = bound_renderbuffer_->height();
    break;
  default:
    glGetRenderbufferParameterivEXT(target, pname, params);
    break;
  }
}
