error::Error GLES2DecoderPassthroughImpl::DoStencilOpSeparate(GLenum face,
                                                              GLenum fail,
                                                              GLenum zfail,
                                                              GLenum zpass) {
  api()->glStencilOpSeparateFn(face, fail, zfail, zpass);
  return error::kNoError;
}
