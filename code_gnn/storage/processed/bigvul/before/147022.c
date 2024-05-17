ScrollableArea* WebLocalFrameImpl::LayoutViewportScrollableArea() const {
  if (LocalFrameView* view = GetFrameView())
    return view->LayoutViewportScrollableArea();
  return nullptr;
}
