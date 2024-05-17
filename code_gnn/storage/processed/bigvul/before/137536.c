bool PrintWebViewHelper::PrintPreviewContext::HasSelection() {
  return IsModifiable() && source_frame()->hasSelection();
}
