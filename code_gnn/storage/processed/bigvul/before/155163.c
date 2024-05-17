bool OmniboxViewViews::SelectionAtBeginning() const {
  const gfx::Range sel = GetSelectedRange();
  return sel.GetMax() == 0;
}
