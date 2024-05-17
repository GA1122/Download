bool TabStrip::MaySetClip() {
  return touch_layout_ || drag_context_->IsStackingDraggedTabs();
}
