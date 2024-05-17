void GLES2DecoderImpl::DoSamplerParameteri(
    GLuint client_id, GLenum pname, GLint param) {
  Sampler* sampler = GetSampler(client_id);
  if (!sampler) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glSamplerParameteri", "unknown sampler");
    return;
  }
  sampler_manager()->SetParameteri(
      "glSamplerParameteri", GetErrorState(), sampler, pname, param);
}
