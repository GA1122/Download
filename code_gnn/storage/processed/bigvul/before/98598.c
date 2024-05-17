int DraggedTabGtk::ScaleValue(int value) {
  return attached_ ? value : static_cast<int>(value * kScalingFactor);
}
