WebSize WebLocalFrameImpl::GetScrollOffset() const {
  if (ScrollableArea* scrollable_area = LayoutViewportScrollableArea())
    return scrollable_area->ScrollOffsetInt();
  return WebSize();
}
