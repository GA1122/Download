void GLES2DecoderImpl::DoGetSamplerParameteriv(GLuint client_id,
                                               GLenum pname,
                                               GLint* params,
                                               GLsizei params_size) {
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glGetSamplerParamteriv", "unknown sampler");
    return;
  }
  api()->glGetSamplerParameterivFn(sampler->service_id(), pname, params);
}
