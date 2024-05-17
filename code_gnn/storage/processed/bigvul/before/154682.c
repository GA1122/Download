error::Error GLES2DecoderPassthroughImpl::DoGetError(uint32_t* result) {
  FlushErrors();
  *result = PopError();
  return error::kNoError;
}
