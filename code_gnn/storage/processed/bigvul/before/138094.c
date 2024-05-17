float AXNodeObject::minValueForRange() const {
  if (hasAttribute(aria_valueminAttr))
    return getAttribute(aria_valueminAttr).toFloat();

  if (isNativeSlider())
    return toHTMLInputElement(*getNode()).minimum();

  if (isHTMLMeterElement(getNode()))
    return toHTMLMeterElement(*getNode()).min();

  return 0.0;
}
