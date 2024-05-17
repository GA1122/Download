void GLES2DecoderImpl::DoGetTexParameterfv(GLenum target,
                                           GLenum pname,
                                           GLfloat* params,
                                           GLsizei params_size) {
  GetTexParameterImpl(target, pname, params, nullptr, "glGetTexParameterfv");
}
