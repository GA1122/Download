TextStyle AXLayoutObject::getTextStyle() const {
  if (!getLayoutObject())
    return AXNodeObject::getTextStyle();

  const ComputedStyle* style = getLayoutObject()->style();
  if (!style)
    return AXNodeObject::getTextStyle();

  unsigned textStyle = TextStyleNone;
  if (style->fontWeight() == FontWeightBold)
    textStyle |= TextStyleBold;
  if (style->getFontDescription().style() == FontStyleItalic)
    textStyle |= TextStyleItalic;
  if (style->getTextDecoration() == TextDecorationUnderline)
    textStyle |= TextStyleUnderline;
  if (style->getTextDecoration() == TextDecorationLineThrough)
    textStyle |= TextStyleLineThrough;

  return static_cast<TextStyle>(textStyle);
}
