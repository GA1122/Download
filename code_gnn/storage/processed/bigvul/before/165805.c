bool SVGElement::IsPresentationAttribute(const QualifiedName& name) const {
  if (const SVGAnimatedPropertyBase* property = PropertyFromAttribute(name))
    return property->HasPresentationAttributeMapping();
  return CssPropertyIdForSVGAttributeName(name) > 0;
}
