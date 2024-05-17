float AXLayoutObject::fontSize() const {
  if (!getLayoutObject())
    return AXNodeObject::fontSize();

  const ComputedStyle* style = getLayoutObject()->style();
  if (!style)
    return AXNodeObject::fontSize();

  return style->computedFontSize();
}
