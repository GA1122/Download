error::Error GLES2DecoderPassthroughImpl::DoBlendEquationSeparate(
    GLenum modeRGB,
    GLenum modeAlpha) {
  api()->glBlendEquationSeparateFn(modeRGB, modeAlpha);
  return error::kNoError;
}
