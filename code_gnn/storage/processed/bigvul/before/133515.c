scoped_ptr<EdgeEffectBase> CreateGlowEdgeEffect(
    ui::ResourceManager* resource_manager,
    float dpi_scale) {
  DCHECK(resource_manager);
  if (IsAndroidLOrNewer())
    return scoped_ptr<EdgeEffectBase>(new EdgeEffectL(resource_manager));

  return scoped_ptr<EdgeEffectBase>(
      new EdgeEffect(resource_manager, dpi_scale));
}
