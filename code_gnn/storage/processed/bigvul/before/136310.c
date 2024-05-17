static bool NeedsTransform(const LayoutObject& object) {
  if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled() &&
      object.StyleRef().BackfaceVisibility() == EBackfaceVisibility::kHidden)
    return true;

  if (!object.IsBox())
    return false;
  return object.StyleRef().HasTransform() || object.StyleRef().Preserves3D() ||
         CompositingReasonsForTransform(ToLayoutBox(object)) !=
             CompositingReason::kNone;
}
