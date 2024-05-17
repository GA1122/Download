bool FragmentPaintPropertyTreeBuilder::NeedsOverflowControlsClip() const {
  if (!object_.HasOverflowClip())
    return false;

  const auto& box = ToLayoutBox(object_);
  const auto* scrollable_area = box.GetScrollableArea();
  IntRect scroll_controls_bounds =
      scrollable_area->ScrollCornerAndResizerRect();
  if (const auto* scrollbar = scrollable_area->HorizontalScrollbar())
    scroll_controls_bounds.Unite(scrollbar->FrameRect());
  if (const auto* scrollbar = scrollable_area->VerticalScrollbar())
    scroll_controls_bounds.Unite(scrollbar->FrameRect());
  auto pixel_snapped_border_box_rect = box.PixelSnappedBorderBoxRect(
      ToLayoutSize(context_.current.paint_offset));
  pixel_snapped_border_box_rect.SetLocation(IntPoint());
  return !pixel_snapped_border_box_rect.Contains(scroll_controls_bounds);
}
