void OmniboxViewViews::UpdatePopup() {
  const gfx::Range sel = GetSelectedRange();
  model()->UpdateInput(!sel.is_empty(), !SelectionAtEnd());
}
