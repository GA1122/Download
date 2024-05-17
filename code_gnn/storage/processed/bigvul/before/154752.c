error::Error GLES2DecoderPassthroughImpl::DoMemoryBarrierEXT(
    GLbitfield barriers) {
  api()->glMemoryBarrierEXTFn(barriers);
  return error::kNoError;
}
