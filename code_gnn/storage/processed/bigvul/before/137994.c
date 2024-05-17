AccessibilityTextDirection AXLayoutObject::textDirection() const {
  if (!getLayoutObject())
    return AXNodeObject::textDirection();

  const ComputedStyle* style = getLayoutObject()->style();
  if (!style)
    return AXNodeObject::textDirection();

  if (style->isHorizontalWritingMode()) {
    switch (style->direction()) {
      case TextDirection::kLtr:
        return AccessibilityTextDirectionLTR;
      case TextDirection::kRtl:
        return AccessibilityTextDirectionRTL;
    }
  } else {
    switch (style->direction()) {
      case TextDirection::kLtr:
        return AccessibilityTextDirectionTTB;
      case TextDirection::kRtl:
        return AccessibilityTextDirectionBTT;
    }
  }

  return AXNodeObject::textDirection();
}
