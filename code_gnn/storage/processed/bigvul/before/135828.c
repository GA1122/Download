void SelectionEditor::DocumentAttached(Document* document) {
  DCHECK(document);
  DCHECK(!LifecycleContext()) << LifecycleContext();
  style_version_for_dom_tree_ = static_cast<uint64_t>(-1);
  style_version_for_flat_tree_ = static_cast<uint64_t>(-1);
  ClearVisibleSelection();
  SetContext(document);
}
