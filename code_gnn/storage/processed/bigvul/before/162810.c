void ContextState::RestoreActiveTextureUnitBinding(unsigned int target) const {
  DCHECK_LT(active_texture_unit, texture_units.size());
  const TextureUnit& texture_unit = texture_units[active_texture_unit];
  if (TargetIsSupported(feature_info_, target))
    api()->glBindTextureFn(target, GetServiceId(texture_unit, target));
}
