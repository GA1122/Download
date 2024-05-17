 void GLES2DecoderImpl::DoTexStorage2DEXT(GLenum target,
                                         GLsizei levels,
                                         GLenum internal_format,
                                         GLsizei width,
                                         GLsizei height) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoTexStorage2D",
      "width", width, "height", height);
  TexStorageImpl(target, levels, internal_format, width, height, 1,
                 ContextState::k2D, "glTexStorage2D");
}
