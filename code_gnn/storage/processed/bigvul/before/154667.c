error::Error GLES2DecoderPassthroughImpl::DoGenSamplers(
    GLsizei n,
    volatile GLuint* samplers) {
  return GenHelper(n, samplers, &resources_->sampler_id_map,
                   [this](GLsizei n, GLuint* samplers) {
                     api()->glGenSamplersFn(n, samplers);
                   });
}
