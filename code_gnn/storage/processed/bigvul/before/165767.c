void SVGElement::ApplyActiveWebAnimations() {
  ActiveInterpolationsMap active_interpolations_map =
      EffectStack::ActiveInterpolations(
          &GetElementAnimations()->GetEffectStack(), nullptr, nullptr,
          KeyframeEffect::kDefaultPriority, IsSVGAttributeHandle);
  for (auto& entry : active_interpolations_map) {
    const QualifiedName& attribute = entry.key.SvgAttribute();
    SVGInterpolationTypesMap map;
    SVGInterpolationEnvironment environment(
        map, *this, PropertyFromAttribute(attribute)->BaseValueBase());
    InvalidatableInterpolation::ApplyStack(entry.value, environment);
  }
  if (!HasSVGRareData())
    return;
  SvgRareData()->SetWebAnimatedAttributesDirty(false);
}
