LayoutRect PaintLayerScrollableArea::ScrollIntoView(
    const LayoutRect& absolute_rect,
    const WebScrollIntoViewParams& params) {
  LayoutRect local_expose_rect =
      AbsoluteToLocal(*GetLayoutBox(), absolute_rect);
  LayoutSize border_origin_to_scroll_origin =
      LayoutSize(-GetLayoutBox()->BorderLeft(), -GetLayoutBox()->BorderTop()) +
      LayoutSize(GetScrollOffset());
  local_expose_rect.Move(border_origin_to_scroll_origin);
  LayoutRect scroll_snapport_rect = VisibleScrollSnapportRect();

  ScrollOffset target_offset = ScrollAlignment::GetScrollOffsetToExpose(
      scroll_snapport_rect, local_expose_rect, params.GetScrollAlignmentX(),
      params.GetScrollAlignmentY(), GetScrollOffset());
  ScrollOffset new_scroll_offset(
      ClampScrollOffset(RoundedIntSize(target_offset)));

  ScrollOffset old_scroll_offset = GetScrollOffset();
  if (params.GetScrollType() == kUserScroll) {
    if (!UserInputScrollable(kHorizontalScrollbar))
      new_scroll_offset.SetWidth(old_scroll_offset.Width());
    if (!UserInputScrollable(kVerticalScrollbar))
      new_scroll_offset.SetHeight(old_scroll_offset.Height());
  }

  FloatPoint end_point = ScrollOffsetToPosition(new_scroll_offset);
  std::unique_ptr<SnapSelectionStrategy> strategy =
      SnapSelectionStrategy::CreateForEndPosition(gfx::ScrollOffset(end_point),
                                                  true, true);
  end_point = GetLayoutBox()
                  ->GetDocument()
                  .GetSnapCoordinator()
                  ->GetSnapPosition(*GetLayoutBox(), *strategy)
                  .value_or(end_point);
  new_scroll_offset = ScrollPositionToOffset(end_point);

  if (params.is_for_scroll_sequence) {
    DCHECK(params.GetScrollType() == kProgrammaticScroll ||
           params.GetScrollType() == kUserScroll);
    ScrollBehavior behavior =
        DetermineScrollBehavior(params.GetScrollBehavior(),
                                GetLayoutBox()->StyleRef().GetScrollBehavior());
    GetSmoothScrollSequencer()->QueueAnimation(this, new_scroll_offset,
                                               behavior);
  } else {
    SetScrollOffset(new_scroll_offset, params.GetScrollType(),
                    kScrollBehaviorInstant);
  }

  ScrollOffset scroll_offset_difference = new_scroll_offset - old_scroll_offset;
  local_expose_rect.Move(-LayoutSize(scroll_offset_difference));

  local_expose_rect.Move(-border_origin_to_scroll_origin);
  scroll_snapport_rect.Move(-border_origin_to_scroll_origin);
  LayoutRect intersect = Intersection(scroll_snapport_rect, local_expose_rect);

  if (intersect.IsEmpty() && !scroll_snapport_rect.IsEmpty() &&
      !local_expose_rect.IsEmpty()) {
    return LocalToAbsolute(*GetLayoutBox(), local_expose_rect);
  }
  intersect = LocalToAbsolute(*GetLayoutBox(), intersect);
  return intersect;
}
