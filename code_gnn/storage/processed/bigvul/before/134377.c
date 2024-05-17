bool TabStrip::IsTabStripCloseable() const {
  return !IsDragSessionActive();
}
