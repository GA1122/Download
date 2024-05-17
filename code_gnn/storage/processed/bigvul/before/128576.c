int Instance::GetScrollbarThickness() {
  if (scrollbar_thickness_ == -1) {
    pp::Scrollbar_Dev temp_scrollbar(this, false);
    scrollbar_thickness_ = temp_scrollbar.GetThickness();
    scrollbar_reserved_thickness_ =
      temp_scrollbar.IsOverlay() ? 0 : scrollbar_thickness_;
  }

  return scrollbar_thickness_;
}
