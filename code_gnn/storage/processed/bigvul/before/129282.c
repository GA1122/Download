void GLES2DecoderImpl::DoGetBufferParameteriv(
    GLenum target, GLenum pname, GLint* params) {
  buffer_manager()->ValidateAndDoGetBufferParameteriv(
      &state_, target, pname, params);
}
