error::Error GLES2DecoderPassthroughImpl::DoReleaseShaderCompiler() {
  api()->glReleaseShaderCompilerFn();
  return error::kNoError;
}
