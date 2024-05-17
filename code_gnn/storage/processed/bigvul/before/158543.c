WebSize WebLocalFrameImpl::GetScrollOffset() const {
  if (ScrollableArea* scrollable_area = LayoutViewport())
    return scrollable_area->ScrollOffsetInt();
  return WebSize();
}
