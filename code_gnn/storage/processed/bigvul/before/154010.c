void GLES2DecoderImpl::DoGetInteger64i_v(GLenum target,
                                         GLuint index,
                                         GLint64* params,
                                         GLsizei params_size) {
  GetIndexedIntegerImpl<GLint64>("glGetInteger64i_v", target, index, params);
}
