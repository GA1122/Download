void GLES2DecoderImpl::DeleteSamplersHelper(GLsizei n,
                                            const volatile GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    GLuint client_id = client_ids[ii];
    Sampler* sampler = GetSampler(client_id);
    if (sampler && !sampler->IsDeleted()) {
      state_.UnbindSampler(sampler);

      RemoveSampler(client_id);
    }
  }
}
