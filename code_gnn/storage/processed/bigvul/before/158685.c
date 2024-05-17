void GLES2DecoderImpl::DoGetBufferParameteri64v(GLenum target,
                                                GLenum pname,
                                                GLint64* params,
                                                GLsizei params_size) {
  buffer_manager()->ValidateAndDoGetBufferParameteri64v(
      &state_, target, pname, params);
}
