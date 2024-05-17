void TabStripModel::SetSelectionFromModel(
    const TabStripSelectionModel& source) {
  DCHECK_NE(TabStripSelectionModel::kUnselectedIndex, source.active());
  int old_active_index = active_index();
  selection_model_.Copy(source);
  NotifySelectionChanged(old_active_index);
}
