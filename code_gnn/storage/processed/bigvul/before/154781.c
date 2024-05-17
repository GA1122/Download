error::Error GLES2DecoderPassthroughImpl::DoSampleCoverage(GLclampf value,
                                                           GLboolean invert) {
  api()->glSampleCoverageFn(value, invert);
  return error::kNoError;
}
