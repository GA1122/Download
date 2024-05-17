gfx::Insets GM2TabStyle::GetContentsInsets() const {
  const int stroke_thickness = GetStrokeThickness();
  const int horizontal_inset = GetContentsHorizontalInsetSize();
  return gfx::Insets(
      stroke_thickness, horizontal_inset,
      stroke_thickness + GetLayoutConstant(TABSTRIP_TOOLBAR_OVERLAP),
      horizontal_inset);
}
