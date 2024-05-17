void GLES2DecoderImpl::DoSamplerParameterfv(GLuint client_id,
                                            GLenum pname,
                                            const volatile GLfloat* params) {
  DCHECK(params);
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glSamplerParameterfv", "unknown sampler");
    return;
  }
  sampler_manager()->SetParameterf("glSamplerParameterfv", error_state_.get(),
                                   sampler, pname, params[0]);
}
