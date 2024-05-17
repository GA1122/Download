bool OffscreenCanvas::IsOpaque() const {
  return context_ ? !context_->CreationAttributes().alpha : false;
}
