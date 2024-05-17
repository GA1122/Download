void ContextState::UnbindSampler(Sampler* sampler) {
  for (size_t jj = 0; jj < sampler_units.size(); ++jj) {
    if (sampler_units[jj].get() == sampler) {
      sampler_units[jj] = nullptr;
      api()->glBindSamplerFn(jj, 0);
    }
  }
}
