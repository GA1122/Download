error::Error GLES2DecoderPassthroughImpl::DoBlendColor(GLclampf red,
                                                       GLclampf green,
                                                       GLclampf blue,
                                                       GLclampf alpha) {
  api()->glBlendColorFn(red, green, blue, alpha);
  return error::kNoError;
}
