int Instance::GetToolbarBottomOffset() {
  int scrollbar_thickness = GetScrollbarThickness();
  return GetScaled(kToolbarFadingOffsetBottom) + scrollbar_thickness;
}
