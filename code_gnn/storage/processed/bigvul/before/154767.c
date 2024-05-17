error::Error GLES2DecoderPassthroughImpl::DoPushGroupMarkerEXT(
    GLsizei length,
    const char* marker) {
  api()->glPushGroupMarkerEXTFn(length, marker);
  return error::kNoError;
}
