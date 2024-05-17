void TreeView::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_OUTLINE;
  state->state = ui::AccessibilityTypes::STATE_READONLY;
}
