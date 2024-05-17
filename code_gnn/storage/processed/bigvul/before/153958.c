void GLES2DecoderImpl::DoBindTexImage2DCHROMIUM(
    GLenum target, GLint image_id) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoBindTexImage2DCHROMIUM");

  BindTexImage2DCHROMIUMImpl("glBindTexImage2DCHROMIUM", target, 0, image_id);
}
