WebRect WebLocalFrameImpl::VisibleContentRect() const {
  if (LocalFrameView* view = GetFrameView())
    return view->VisibleContentRect();
  return WebRect();
}
