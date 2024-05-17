error::Error GLES2DecoderPassthroughImpl::DoStencilFunc(GLenum func,
                                                        GLint ref,
                                                        GLuint mask) {
  api()->glStencilFuncFn(func, ref, mask);
  return error::kNoError;
}
