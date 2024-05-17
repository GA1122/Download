static bool CanOmitOverflowClip(const LayoutObject& object) {
  DCHECK(NeedsOverflowClip(object));
  if (!object.IsLayoutBlock() || object.IsSVG())
    return false;

  const auto& block = ToLayoutBlock(object);
  if (block.HasLayer() && block.Layer()->FirstChild())
    return false;
  if (block.GetSelectionState() != SelectionState::kNone)
    return false;
  if (block.HasControlClip() || block.ShouldPaintCarets())
    return false;

  auto clip_rect = block.OverflowClipRect(LayoutPoint());
  auto clip_rect_excluding_overlay_scrollbars = block.OverflowClipRect(
      LayoutPoint(), kExcludeOverlayScrollbarSizeForHitTesting);
  if (clip_rect != clip_rect_excluding_overlay_scrollbars)
    return false;
  return clip_rect.Contains(block.ContentsVisualOverflowRect());
}
