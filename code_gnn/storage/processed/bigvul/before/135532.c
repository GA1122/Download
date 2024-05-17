void Editor::Clear() {
  should_style_with_css_ = false;
  default_paragraph_separator_ = kEditorParagraphSeparatorIsDiv;
  last_edit_command_ = nullptr;
  undo_stack_->Clear();
}
