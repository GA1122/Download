error::Error GLES2DecoderPassthroughImpl::DoStencilMaskSeparate(GLenum face,
                                                                GLuint mask) {
  api()->glStencilMaskSeparateFn(face, mask);
  return error::kNoError;
}
