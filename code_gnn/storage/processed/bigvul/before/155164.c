bool OmniboxViewViews::SelectionAtEnd() const {
  const gfx::Range sel = GetSelectedRange();
  return sel.GetMin() == text().size();
}
