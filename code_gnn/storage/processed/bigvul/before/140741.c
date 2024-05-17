void GLES2DecoderImpl::DoGetRenderbufferParameteriv(
    GLenum target, GLenum pname, GLint* params) {
  Renderbuffer* renderbuffer =
      GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
  if (!renderbuffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glGetRenderbufferParameteriv", "no renderbuffer bound");
    return;
  }

  EnsureRenderbufferBound();
  switch (pname) {
    case GL_RENDERBUFFER_INTERNAL_FORMAT:
      *params = renderbuffer->internal_format();
      break;
    case GL_RENDERBUFFER_WIDTH:
      *params = renderbuffer->width();
      break;
    case GL_RENDERBUFFER_HEIGHT:
      *params = renderbuffer->height();
      break;
    case GL_RENDERBUFFER_SAMPLES_EXT:
      if (features().use_img_for_multisampled_render_to_texture) {
        glGetRenderbufferParameterivEXT(target, GL_RENDERBUFFER_SAMPLES_IMG,
            params);
      } else {
        glGetRenderbufferParameterivEXT(target, GL_RENDERBUFFER_SAMPLES_EXT,
            params);
      }
    default:
      glGetRenderbufferParameterivEXT(target, pname, params);
      break;
  }
}
