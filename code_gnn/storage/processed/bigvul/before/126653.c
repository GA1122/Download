void TabStripModel::SetSelectionFromModel(
    const TabStripSelectionModel& source) {
  DCHECK_NE(TabStripSelectionModel::kUnselectedIndex, source.active());
  SetSelection(source, NOTIFY_DEFAULT);
}
