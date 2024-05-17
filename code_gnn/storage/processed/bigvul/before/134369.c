bool TabStrip::IsDragSessionActive() const {
  return drag_controller_.get() != NULL;
}
