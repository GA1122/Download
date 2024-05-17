void HTMLElement::applyBorderAttributeToStyle(const Attribute& attribute, StylePropertySet* style)
{
    addPropertyToAttributeStyle(style, CSSPropertyBorderWidth, parseBorderWidthAttribute(attribute), CSSPrimitiveValue::CSS_PX);
    addPropertyToAttributeStyle(style, CSSPropertyBorderStyle, CSSValueSolid);
}
