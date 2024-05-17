error::Error GLES2DecoderPassthroughImpl::DoBindTexImage2DCHROMIUM(
    GLenum target,
    GLint imageId) {
  return BindTexImage2DCHROMIUMImpl(target, 0, imageId);
}
