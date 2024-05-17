void GLES2DecoderImpl::DoGetSamplerParameterfv(GLuint client_id,
                                               GLenum pname,
                                               GLfloat* params,
                                               GLsizei params_size) {
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glGetSamplerParamterfv", "unknown sampler");
    return;
  }
  api()->glGetSamplerParameterfvFn(sampler->service_id(), pname, params);
}
