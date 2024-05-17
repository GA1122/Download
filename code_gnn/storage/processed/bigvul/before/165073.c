void OffscreenCanvas::PushFrameIfNeeded() {
  if (needs_push_frame_ && context_) {
    context_->PushFrame();
  }
}
