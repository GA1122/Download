bool OmniboxViewViews::IsSelectAll() const {
  return !text().empty() && text() == GetSelectedText();
}
