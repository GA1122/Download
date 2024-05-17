WebSize WebLocalFrameImpl::DocumentSize() const {
  if (!GetFrameView() || !GetFrameView()->GetLayoutView())
    return WebSize();

  return GetFrameView()->GetLayoutView()->DocumentRect().Size();
}
