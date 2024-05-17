bool GLES2DecoderImpl::DoIsSampler(GLuint client_id) {
  const Sampler* sampler = GetSampler(client_id);
  return sampler && !sampler->IsDeleted();
}
