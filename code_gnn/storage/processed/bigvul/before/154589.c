error::Error GLES2DecoderPassthroughImpl::DoClearColor(GLclampf red,
                                                       GLclampf green,
                                                       GLclampf blue,
                                                       GLclampf alpha) {
  api()->glClearColorFn(red, green, blue, alpha);
  return error::kNoError;
}
