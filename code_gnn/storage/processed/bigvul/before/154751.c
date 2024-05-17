error::Error GLES2DecoderPassthroughImpl::DoMemoryBarrierByRegion(
    GLbitfield barriers) {
  api()->glMemoryBarrierByRegionFn(barriers);
  return error::kNoError;
}
