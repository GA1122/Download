void OmniboxViewViews::OnFocus() {
  views::Textfield::OnFocus();
  model()->OnSetFocus(false);

  if (saved_selection_for_focus_change_.IsValid()) {
    SelectRange(saved_selection_for_focus_change_);
    saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  }
}
