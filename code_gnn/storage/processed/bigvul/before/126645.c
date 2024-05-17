void TabStripModel::NotifyIfActiveOrSelectionChanged(
    TabContents* old_contents,
    NotifyTypes notify_types,
    const TabStripSelectionModel& old_model) {
  NotifyIfActiveTabChanged(old_contents, notify_types);

  if (!selection_model().Equals(old_model)) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabSelectionChanged(this, old_model));
  }
}
