bool Editor::CanUndo() {
  return undo_stack_->CanUndo();
}
