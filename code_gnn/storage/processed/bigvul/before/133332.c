bool PaletteTray::PerformAction(const ui::Event& event) {
  if (bubble_) {
    bubble_.reset();
    return true;
  }

  return OpenBubble();
}
