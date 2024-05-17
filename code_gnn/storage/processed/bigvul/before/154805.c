error::Error GLES2DecoderPassthroughImpl::DoStencilOp(GLenum fail,
                                                      GLenum zfail,
                                                      GLenum zpass) {
  api()->glStencilOpFn(fail, zfail, zpass);
  return error::kNoError;
}
