float AXNodeObject::maxValueForRange() const {
  if (hasAttribute(aria_valuemaxAttr))
    return getAttribute(aria_valuemaxAttr).toFloat();

  if (isNativeSlider())
    return toHTMLInputElement(*getNode()).maximum();

  if (isHTMLMeterElement(getNode()))
    return toHTMLMeterElement(*getNode()).max();

  return 0.0;
}
