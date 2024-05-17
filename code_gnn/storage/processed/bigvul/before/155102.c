void OmniboxViewViews::ApplyCaretVisibility() {
  SetCursorEnabled(model()->is_caret_visible());

  if (location_bar_view_)
    location_bar_view_->OnOmniboxFocused();
}
