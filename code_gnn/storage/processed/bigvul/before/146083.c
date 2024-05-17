void WebGL2RenderingContextBase::deleteSampler(WebGLSampler* sampler) {
  if (isContextLost())
    return;

  for (size_t i = 0; i < sampler_units_.size(); ++i) {
    if (sampler == sampler_units_[i]) {
      sampler_units_[i] = nullptr;
      ContextGL()->BindSampler(i, 0);
    }
  }

  DeleteObject(sampler);
}
