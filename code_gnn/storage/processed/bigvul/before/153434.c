bool TabStrip::IsTabStripCloseable() const {
  return !drag_context_->IsDragSessionActive();
}
