error::Error GLES2DecoderPassthroughImpl::DoBlendFuncSeparate(GLenum srcRGB,
                                                              GLenum dstRGB,
                                                              GLenum srcAlpha,
                                                              GLenum dstAlpha) {
  api()->glBlendFuncSeparateFn(srcRGB, dstRGB, srcAlpha, dstAlpha);
  return error::kNoError;
}
