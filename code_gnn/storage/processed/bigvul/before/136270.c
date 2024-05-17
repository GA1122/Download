void BlockPainter::Paint(const PaintInfo& paint_info,
                         const LayoutPoint& paint_offset) {
  AdjustPaintOffsetScope adjustment(layout_block_, paint_info, paint_offset);
  auto adjusted_paint_offset = adjustment.AdjustedPaintOffset();
  auto& local_paint_info = adjustment.MutablePaintInfo();

  if (!IntersectsPaintRect(local_paint_info, adjusted_paint_offset))
    return;

  PaintPhase original_phase = local_paint_info.phase;

  ContentsClipBehavior contents_clip_behavior = kForceContentsClip;
  if (layout_block_.ShouldClipOverflow() && !layout_block_.HasControlClip() &&
      !layout_block_.ShouldPaintCarets())
    contents_clip_behavior = kSkipContentsClipIfPossible;

  if (original_phase == PaintPhase::kOutline) {
    local_paint_info.phase = PaintPhase::kDescendantOutlinesOnly;
  } else if (ShouldPaintSelfBlockBackground(original_phase)) {
    local_paint_info.phase = PaintPhase::kSelfBlockBackgroundOnly;
    layout_block_.PaintObject(local_paint_info, adjusted_paint_offset);
    if (ShouldPaintDescendantBlockBackgrounds(original_phase))
      local_paint_info.phase = PaintPhase::kDescendantBlockBackgroundsOnly;
  }

  if (original_phase != PaintPhase::kSelfBlockBackgroundOnly &&
      original_phase != PaintPhase::kSelfOutlineOnly) {
    base::Optional<BoxClipper> clipper;
    if (!layout_block_.IsSVGForeignObject() ||
        RuntimeEnabledFeatures::SlimmingPaintV175Enabled()) {
      clipper.emplace(layout_block_, local_paint_info, adjusted_paint_offset,
                      contents_clip_behavior);
    }
    layout_block_.PaintObject(local_paint_info, adjusted_paint_offset);
  }

  if (ShouldPaintSelfOutline(original_phase)) {
    local_paint_info.phase = PaintPhase::kSelfOutlineOnly;
    layout_block_.PaintObject(local_paint_info, adjusted_paint_offset);
  }

  local_paint_info.phase = original_phase;
  PaintOverflowControlsIfNeeded(local_paint_info, adjusted_paint_offset);
}
