AnimatedPropertyType SVGElement::AnimatedPropertyTypeForCSSAttribute(
    const QualifiedName& attribute_name) {
  DEFINE_STATIC_LOCAL(AttributeToPropertyTypeMap, css_property_map, ());

  if (css_property_map.IsEmpty()) {
    struct AttrToTypeEntry {
      const QualifiedName& attr;
      const AnimatedPropertyType prop_type;
    };
    const AttrToTypeEntry attr_to_types[] = {
        {kAlignmentBaselineAttr, kAnimatedString},
        {kBaselineShiftAttr, kAnimatedString},
        {kBufferedRenderingAttr, kAnimatedString},
        {kClipPathAttr, kAnimatedString},
        {kClipRuleAttr, kAnimatedString},
        {svg_names::kColorAttr, kAnimatedColor},
        {kColorInterpolationAttr, kAnimatedString},
        {kColorInterpolationFiltersAttr, kAnimatedString},
        {kColorRenderingAttr, kAnimatedString},
        {kCursorAttr, kAnimatedString},
        {kDisplayAttr, kAnimatedString},
        {kDominantBaselineAttr, kAnimatedString},
        {kFillAttr, kAnimatedColor},
        {kFillOpacityAttr, kAnimatedNumber},
        {kFillRuleAttr, kAnimatedString},
        {kFilterAttr, kAnimatedString},
        {kFloodColorAttr, kAnimatedColor},
        {kFloodOpacityAttr, kAnimatedNumber},
        {kFontFamilyAttr, kAnimatedString},
        {kFontSizeAttr, kAnimatedLength},
        {kFontStretchAttr, kAnimatedString},
        {kFontStyleAttr, kAnimatedString},
        {kFontVariantAttr, kAnimatedString},
        {kFontWeightAttr, kAnimatedString},
        {kImageRenderingAttr, kAnimatedString},
        {kLetterSpacingAttr, kAnimatedLength},
        {kLightingColorAttr, kAnimatedColor},
        {kMarkerEndAttr, kAnimatedString},
        {kMarkerMidAttr, kAnimatedString},
        {kMarkerStartAttr, kAnimatedString},
        {kMaskAttr, kAnimatedString},
        {kMaskTypeAttr, kAnimatedString},
        {kOpacityAttr, kAnimatedNumber},
        {kOverflowAttr, kAnimatedString},
        {kPaintOrderAttr, kAnimatedString},
        {kPointerEventsAttr, kAnimatedString},
        {kShapeRenderingAttr, kAnimatedString},
        {kStopColorAttr, kAnimatedColor},
        {kStopOpacityAttr, kAnimatedNumber},
        {kStrokeAttr, kAnimatedColor},
        {kStrokeDasharrayAttr, kAnimatedLengthList},
        {kStrokeDashoffsetAttr, kAnimatedLength},
        {kStrokeLinecapAttr, kAnimatedString},
        {kStrokeLinejoinAttr, kAnimatedString},
        {kStrokeMiterlimitAttr, kAnimatedNumber},
        {kStrokeOpacityAttr, kAnimatedNumber},
        {kStrokeWidthAttr, kAnimatedLength},
        {kTextAnchorAttr, kAnimatedString},
        {kTextDecorationAttr, kAnimatedString},
        {kTextRenderingAttr, kAnimatedString},
        {kVectorEffectAttr, kAnimatedString},
        {kVisibilityAttr, kAnimatedString},
        {kWordSpacingAttr, kAnimatedLength},
    };
    for (size_t i = 0; i < arraysize(attr_to_types); i++)
      css_property_map.Set(attr_to_types[i].attr, attr_to_types[i].prop_type);
  }
  return css_property_map.at(attribute_name);
}
