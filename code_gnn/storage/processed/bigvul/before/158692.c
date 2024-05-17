void GLES2DecoderImpl::DoSamplerParameteriv(GLuint client_id,
                                            GLenum pname,
                                            const volatile GLint* params) {
  DCHECK(params);
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glSamplerParameteriv", "unknown sampler");
    return;
  }
  sampler_manager()->SetParameteri(
      "glSamplerParameteriv", GetErrorState(), sampler, pname, params[0]);
}
