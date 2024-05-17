void SVGElement::CollectStyleForPresentationAttribute(
    const QualifiedName& name,
    const AtomicString& value,
    MutableCSSPropertyValueSet* style) {
  CSSPropertyID property_id = CssPropertyIdForSVGAttributeName(name);
  if (property_id > 0)
    AddPropertyToPresentationAttributeStyle(style, property_id, value);
}
