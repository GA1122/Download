void Textfield::SetInvalid(bool invalid) {
  if (invalid == invalid_)
    return;
  invalid_ = invalid;
  UpdateBorder();

  if (HasFocus() && use_focus_ring_) {
    FocusRing::Install(this, invalid_
                                 ? ui::NativeTheme::kColorId_AlertSeverityHigh
                                 : ui::NativeTheme::kColorId_NumColors);
  }
}
