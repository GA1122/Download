bool Editor::IsSelectTrailingWhitespaceEnabled() const {
  if (Settings* settings = GetFrame().GetSettings())
    return settings->GetSelectTrailingWhitespaceEnabled();
  return false;
}
