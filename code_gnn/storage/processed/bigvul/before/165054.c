void OffscreenCanvas::BeginFrame() {
  DCHECK(HasPlaceholderCanvas());
  PushFrameIfNeeded();
  GetOrCreateResourceDispatcher()->SetNeedsBeginFrame(false);
}
