bool Document::hasFocus() const {
  return GetPage() && GetPage()->GetFocusController().IsDocumentFocused(*this);
}
