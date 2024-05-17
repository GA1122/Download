 MutableCSSPropertyValueSet* SVGElement::AnimatedSMILStyleProperties() const {
  if (HasSVGRareData())
    return SvgRareData()->AnimatedSMILStyleProperties();
  return nullptr;
}
