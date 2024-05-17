error::Error GLES2DecoderPassthroughImpl::DoInsertEventMarkerEXT(
    GLsizei length,
    const char* marker) {
  api()->glInsertEventMarkerEXTFn(length, marker);
  return error::kNoError;
}
