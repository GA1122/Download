void OffscreenCanvas::SetSize(const IntSize& size) {
  if (context_) {
    if (context_->Is3d()) {
      if (size != size_)
        context_->Reshape(size.Width(), size.Height());
    } else if (context_->Is2d()) {
      context_->Reset();
      origin_clean_ = true;
    }
  }
  if (size != size_) {
    UpdateMemoryUsage();
  }
  size_ = size;
  if (frame_dispatcher_)
    frame_dispatcher_->Reshape(size_);

  current_frame_damage_rect_ = SkIRect::MakeWH(size_.Width(), size_.Height());
  if (context_)
    context_->DidDraw();
}
