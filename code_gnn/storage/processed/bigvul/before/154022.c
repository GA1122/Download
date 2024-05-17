void GLES2DecoderImpl::DoGetTexParameteriv(GLenum target,
                                           GLenum pname,
                                           GLint* params,
                                           GLsizei params_size) {
  GetTexParameterImpl(target, pname, nullptr, params, "glGetTexParameteriv");
}
