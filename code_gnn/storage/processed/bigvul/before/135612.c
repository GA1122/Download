Editor::RevealSelectionScope::~RevealSelectionScope() {
  DCHECK(editor_->prevent_reveal_selection_);
  --editor_->prevent_reveal_selection_;
  if (!editor_->prevent_reveal_selection_) {
    editor_->GetFrame().Selection().RevealSelection(
        ScrollAlignment::kAlignToEdgeIfNeeded, kRevealExtent);
  }
}
