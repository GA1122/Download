void TabStripModel::ExtendSelectionTo(int index) {
  DCHECK(ContainsIndex(index));
  int old_active = active_index();
  selection_model_.SetSelectionFromAnchorTo(index);
  NotifySelectionChanged(old_active);
}
