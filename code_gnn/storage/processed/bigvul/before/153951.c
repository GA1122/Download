void GLES2DecoderImpl::DoBindBufferRange(GLenum target, GLuint index,
                                         GLuint client_id,
                                         GLintptr offset,
                                         GLsizeiptr size) {
  BindIndexedBufferImpl(target, index, client_id, offset, size,
                        kBindBufferRange, "glBindBufferRange");
}
