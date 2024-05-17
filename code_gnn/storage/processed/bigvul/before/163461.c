void OmniboxViewViews::UpdatePopup() {
  model()->SetInputInProgress(true);
  if (!model()->has_focus())
    return;

  const gfx::Range sel = GetSelectedRange();
  model()->StartAutocomplete(!sel.is_empty(), sel.GetMax() < text().length());
}
