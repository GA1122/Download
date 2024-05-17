void GLES2DecoderImpl::DoBindBufferBase(GLenum target, GLuint index,
                                        GLuint client_id) {
  BindIndexedBufferImpl(target, index, client_id, 0, 0,
                        kBindBufferBase, "glBindBufferBase");
}
