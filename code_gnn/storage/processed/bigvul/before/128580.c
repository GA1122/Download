int Instance::GetToolbarRightOffset() {
  int scrollbar_thickness = GetScrollbarThickness();
  return GetScaled(kToolbarFadingOffsetRight) + 2 * scrollbar_thickness;
}
