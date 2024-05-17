WebSize WebLocalFrameImpl::ContentsSize() const {
  if (LocalFrameView* view = GetFrameView())
    return view->ContentsSize();
  return WebSize();
}
