error::Error GLES2DecoderPassthroughImpl::DoPopGroupMarkerEXT() {
  api()->glPopGroupMarkerEXTFn();
  return error::kNoError;
}
