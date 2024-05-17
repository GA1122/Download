error::Error GLES2DecoderPassthroughImpl::DoStencilFuncSeparate(GLenum face,
                                                                GLenum func,
                                                                GLint ref,
                                                                GLuint mask) {
  api()->glStencilFuncSeparateFn(face, func, ref, mask);
  return error::kNoError;
}
