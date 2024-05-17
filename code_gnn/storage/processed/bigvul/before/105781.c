gfx::Insets Label::GetInsets() const {
  gfx::Insets insets = View::GetInsets();
  if (IsFocusable() || has_focus_border_)  {
    insets += gfx::Insets(kFocusBorderPadding, kFocusBorderPadding,
                          kFocusBorderPadding, kFocusBorderPadding);
  }
  return insets;
}
