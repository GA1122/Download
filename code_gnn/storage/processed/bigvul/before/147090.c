void WebLocalFrameImpl::SetScrollOffset(const WebSize& offset) {
  if (ScrollableArea* scrollable_area = LayoutViewportScrollableArea()) {
    scrollable_area->SetScrollOffset(ScrollOffset(offset.width, offset.height),
                                     kProgrammaticScroll);
  }
}
