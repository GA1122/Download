void TabStripModel::NotifyTabSelectedIfChanged(TabContentsWrapper* old_contents,
                                               int to_index,
                                               bool user_gesture) {
  TabContentsWrapper* new_contents = GetContentsAt(to_index);
  if (old_contents == new_contents)
    return;

  TabContentsWrapper* last_selected_contents = old_contents;
  if (last_selected_contents) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabDeselected(last_selected_contents));
  }

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabSelectedAt(last_selected_contents, new_contents,
                                  active_index(), user_gesture));
}
