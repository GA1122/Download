Editor::RevealSelectionScope::RevealSelectionScope(Editor* editor)
    : editor_(editor) {
  ++editor_->prevent_reveal_selection_;
}
