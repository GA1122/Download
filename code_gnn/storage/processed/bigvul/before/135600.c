bool Editor::SmartInsertDeleteEnabled() const {
  if (Settings* settings = GetFrame().GetSettings())
    return settings->GetSmartInsertDeleteEnabled();
  return false;
}
