void SelectionEditor::DidFinishTextChange(const Position& new_base,
                                          const Position& new_extent) {
  if (new_base == selection_.base_ && new_extent == selection_.extent_) {
    DidFinishDOMMutation();
    return;
  }
  selection_.base_ = new_base;
  selection_.extent_ = new_extent;
  MarkCacheDirty();
  DidFinishDOMMutation();
}
