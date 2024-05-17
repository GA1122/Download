void ConversionContext::UpdateEffectBounds(
    const FloatRect& bounds,
    const TransformPaintPropertyNode* transform) {
  if (effect_bounds_stack_.IsEmpty() || bounds.IsEmpty())
    return;

  auto& effect_bounds_info = effect_bounds_stack_.back();
  FloatRect mapped_bounds = bounds;
  GeometryMapper::SourceToDestinationRect(
      transform, effect_bounds_info.transform, mapped_bounds);
  effect_bounds_info.bounds.Unite(mapped_bounds);
}
