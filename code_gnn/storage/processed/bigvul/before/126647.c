void TabStripModel::NotifyIfTabDeactivated(TabContents* contents) {
  if (contents) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabDeactivated(contents));
  }
}
