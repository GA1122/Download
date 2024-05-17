void GLES2DecoderImpl::DoSamplerParameterf(
    GLuint client_id, GLenum pname, GLfloat param) {
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glSamplerParameterf", "unknown sampler");
    return;
  }
  sampler_manager()->SetParameterf("glSamplerParameterf", error_state_.get(),
                                   sampler, pname, param);
}
