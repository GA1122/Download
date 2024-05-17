void TabStripModel::ToggleSelectionAt(int index) {
  DCHECK(ContainsIndex(index));
  int old_active = active_index();
  if (selection_model_.IsSelected(index)) {
    if (selection_model_.size() == 1) {
      return;
    }
    selection_model_.RemoveIndexFromSelection(index);
    selection_model_.set_anchor(index);
    if (selection_model_.active() == TabStripSelectionModel::kUnselectedIndex)
      selection_model_.set_active(selection_model_.selected_indices()[0]);
  } else {
    selection_model_.AddIndexToSelection(index);
    selection_model_.set_anchor(index);
    selection_model_.set_active(index);
  }
  NotifySelectionChanged(old_active);
}
