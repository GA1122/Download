bool Editor::CanSmartCopyOrDelete() const {
  return SmartInsertDeleteEnabled() &&
         GetFrame().Selection().Granularity() == TextGranularity::kWord;
}
