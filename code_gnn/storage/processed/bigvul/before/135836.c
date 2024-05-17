void SelectionEditor::SetSelection(const SelectionInDOMTree& new_selection) {
  new_selection.AssertValidFor(GetDocument());
  DCHECK_NE(selection_, new_selection);
  ClearDocumentCachedRange();
  MarkCacheDirty();
  selection_ = new_selection;
}
