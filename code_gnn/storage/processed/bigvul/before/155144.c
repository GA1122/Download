bool OmniboxViewViews::OnInlineAutocompleteTextMaybeChanged(
    const base::string16& display_text,
    size_t user_text_length) {
  if (display_text == text())
    return false;

  if (!IsIMEComposing()) {
    gfx::Range range(display_text.size(), user_text_length);
    SetTextAndSelectedRange(display_text, range);
  } else if (location_bar_view_) {
    location_bar_view_->SetImeInlineAutocompletion(
        display_text.substr(user_text_length));
  }
  TextChanged();
  return true;
}
