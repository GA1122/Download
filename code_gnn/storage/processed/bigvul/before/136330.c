void FragmentPaintPropertyTreeBuilder::UpdateForObjectLocationAndSize(
    base::Optional<IntPoint>& paint_offset_translation) {
#if DCHECK_IS_ON()
  FindPaintOffsetNeedingUpdateScope check_scope(
      object_, fragment_data_, full_context_.is_actually_needed);
#endif

  UpdatePaintOffset();
  UpdateForPaintOffsetTranslation(paint_offset_translation);

  if (fragment_data_.PaintOffset() != context_.current.paint_offset) {
    full_context_.force_subtree_update = true;

    if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled()) {
      if (RoundedIntPoint(fragment_data_.PaintOffset()) ==
          RoundedIntPoint(context_.current.paint_offset)) {
        object_.GetMutableForPainting().SetMayNeedPaintInvalidation();
      } else {
        object_.GetMutableForPainting().SetShouldDoFullPaintInvalidation(
            PaintInvalidationReason::kGeometry);
      }
    }

    fragment_data_.SetPaintOffset(context_.current.paint_offset);
    fragment_data_.InvalidateClipPathCache();
  }

  if (paint_offset_translation)
    context_.current.paint_offset_root = &ToLayoutBoxModelObject(object_);
}
