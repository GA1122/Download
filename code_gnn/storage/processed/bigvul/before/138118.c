float AXNodeObject::valueForRange() const {
  if (hasAttribute(aria_valuenowAttr))
    return getAttribute(aria_valuenowAttr).toFloat();

  if (isNativeSlider())
    return toHTMLInputElement(*getNode()).valueAsNumber();

  if (isHTMLMeterElement(getNode()))
    return toHTMLMeterElement(*getNode()).value();

  return 0.0;
}
