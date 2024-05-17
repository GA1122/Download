void TabStripModel::ToggleSelectionAt(int index) {
  DCHECK(ContainsIndex(index));
  TabStripSelectionModel new_model;
  new_model.Copy(selection_model());
  if (selection_model_.IsSelected(index)) {
    if (selection_model_.size() == 1) {
      return;
    }
    new_model.RemoveIndexFromSelection(index);
    new_model.set_anchor(index);
    if (new_model.active() == index ||
        new_model.active() == TabStripSelectionModel::kUnselectedIndex)
      new_model.set_active(new_model.selected_indices()[0]);
  } else {
    new_model.AddIndexToSelection(index);
    new_model.set_anchor(index);
    new_model.set_active(index);
  }
  SetSelection(new_model, NOTIFY_DEFAULT);
}
