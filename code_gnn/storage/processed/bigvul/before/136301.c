static bool NeedsOverflowClip(const LayoutObject& object) {
  if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled() &&
      object.IsSVGViewportContainer() &&
      SVGLayoutSupport::IsOverflowHidden(object))
    return true;

  return object.IsBox() && ToLayoutBox(object).ShouldClipOverflow() &&
         !IsPrintingRootLayoutView(object);
}
