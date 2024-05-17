void FragmentPaintPropertyTreeBuilder::UpdateForPaintOffsetTranslation(
    base::Optional<IntPoint>& paint_offset_translation) {
  if (!NeedsPaintOffsetTranslation(object_))
    return;

  paint_offset_translation = RoundedIntPoint(context_.current.paint_offset);
  LayoutPoint fractional_paint_offset =
      LayoutPoint(context_.current.paint_offset - *paint_offset_translation);
  if (fractional_paint_offset != LayoutPoint()) {
    TransformationMatrix matrix;
    object_.StyleRef().ApplyTransform(
        matrix, LayoutSize(), ComputedStyle::kExcludeTransformOrigin,
        ComputedStyle::kIncludeMotionPath,
        ComputedStyle::kIncludeIndependentTransformProperties);
    if (!matrix.IsIdentityOrTranslation())
      fractional_paint_offset = LayoutPoint();
  }
  context_.current.paint_offset = fractional_paint_offset;
}
