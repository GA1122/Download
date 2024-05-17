void OffscreenCanvas::DidDraw(const FloatRect& rect) {
  if (rect.IsEmpty())
    return;

  if (HasPlaceholderCanvas()) {
    needs_push_frame_ = true;
    GetOrCreateResourceDispatcher()->SetNeedsBeginFrame(true);
  }
}
