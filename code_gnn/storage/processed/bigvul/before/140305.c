bool Editor::canSmartCopyOrDelete() const {
  return smartInsertDeleteEnabled() &&
         frame().selection().granularity() == WordGranularity;
}
