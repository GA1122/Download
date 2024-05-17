void GLES2DecoderImpl::DoFramebufferParameteri(GLenum target,
                                               GLenum pname,
                                               GLint param) {
  const char* func_name = "glFramebufferParameteri";
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "no framebuffer bound");
    return;
  }
  api()->glFramebufferParameteriFn(target, pname, param);
}
