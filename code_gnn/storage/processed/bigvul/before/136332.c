void FragmentPaintPropertyTreeBuilder::UpdateForSelf() {
  base::Optional<IntPoint> paint_offset_translation;
  UpdateForObjectLocationAndSize(paint_offset_translation);
  if (&fragment_data_ == &object_.FirstFragment())
    SetNeedsPaintPropertyUpdateIfNeeded();
  UpdateClipPathCache();

  if (properties_) {
    UpdateFragmentClip();
    UpdatePaintOffsetTranslation(paint_offset_translation);
  }

#if DCHECK_IS_ON()
  FindObjectPropertiesNeedingUpdateScope check_needs_update_scope(
      object_, fragment_data_, full_context_.force_subtree_update);
#endif

  if (properties_) {
    UpdateTransform();
    UpdateClipPathClip(false);
    if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled())
      UpdateEffect();
    UpdateClipPathClip(true);   
    UpdateCssClip();
    UpdateFilter();
    UpdateOverflowControlsClip();
  }
  UpdateLocalBorderBoxContext();
}
