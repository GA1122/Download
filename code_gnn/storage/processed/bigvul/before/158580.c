void WebLocalFrameImpl::SetScrollOffset(const WebSize& offset) {
  if (ScrollableArea* scrollable_area = LayoutViewport()) {
    scrollable_area->SetScrollOffset(ScrollOffset(offset.width, offset.height),
                                     kProgrammaticScroll);
  }
}
