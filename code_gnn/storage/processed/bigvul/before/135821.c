void SelectionEditor::DidChangeChildren(const ContainerNode&) {
  MarkCacheDirty();
  DidFinishDOMMutation();
}
