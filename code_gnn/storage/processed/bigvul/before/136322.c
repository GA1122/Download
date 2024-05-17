void FragmentPaintPropertyTreeBuilder::SetNeedsPaintPropertyUpdateIfNeeded() {
  if (!object_.IsBox())
    return;

  const LayoutBox& box = ToLayoutBox(object_);

  if (NeedsOverflowClip(box)) {
    bool had_overflow_clip = properties_ && properties_->OverflowClip();
    if (had_overflow_clip == CanOmitOverflowClip(box))
      box.GetMutableForPainting().SetNeedsPaintPropertyUpdate();
  }

  if (box.Size() == box.PreviousSize())
    return;

  const bool box_generates_property_nodes_for_mask_and_clip_path =
      RuntimeEnabledFeatures::SlimmingPaintV175Enabled() &&
      (box.HasMask() || box.HasClipPath());
  if (NeedsOverflowClip(box) || NeedsInnerBorderRadiusClip(box) ||
      NeedsCssClip(box) ||
      box.StyleRef().HasTransform() || NeedsPerspective(box) ||
      box_generates_property_nodes_for_mask_and_clip_path) {
    box.GetMutableForPainting().SetNeedsPaintPropertyUpdate();
  }

  if (box.HasClipPath())
    box.GetMutableForPainting().InvalidateClipPathCache();

  if (box.HasReflection()) {
    DCHECK(box.HasLayer());
    box.Layer()->SetFilterOnEffectNodeDirty();
    box.GetMutableForPainting().SetNeedsPaintPropertyUpdate();
  }
}
