RGBA32 AXLayoutObject::color() const {
  if (!getLayoutObject() || isColorWell())
    return AXNodeObject::color();

  const ComputedStyle* style = getLayoutObject()->style();
  if (!style)
    return AXNodeObject::color();

  Color color = style->visitedDependentColor(CSSPropertyColor);
  return color.rgb();
}
