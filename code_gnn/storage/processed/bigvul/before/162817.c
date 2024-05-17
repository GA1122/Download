 void ContextState::RestoreSamplerBinding(GLuint unit,
                                         const ContextState* prev_state) const {
  if (!feature_info_->IsES3Capable())
    return;
  const scoped_refptr<Sampler>& cur_sampler = sampler_units[unit];
  GLuint cur_id = cur_sampler ? cur_sampler->service_id() : 0;
  GLuint prev_id = 0;
  if (prev_state) {
    const scoped_refptr<Sampler>& prev_sampler =
        prev_state->sampler_units[unit];
    prev_id = prev_sampler ? prev_sampler->service_id() : 0;
  }
  if (!prev_state || cur_id != prev_id) {
    api()->glBindSamplerFn(unit, cur_id);
  }
}
