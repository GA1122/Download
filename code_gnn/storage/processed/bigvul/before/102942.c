void TabStripModel::ActivateTabAt(int index, bool user_gesture) {
  DCHECK(ContainsIndex(index));
  bool had_multi = selection_model_.selected_indices().size() > 1;
  TabContentsWrapper* old_contents =
      (active_index() == TabStripSelectionModel::kUnselectedIndex) ?
      NULL : GetSelectedTabContents();
  selection_model_.SetSelectedIndex(index);
  TabContentsWrapper* new_contents = GetContentsAt(index);
  if (old_contents != new_contents && old_contents) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabDeselected(old_contents));
  }
  if (old_contents != new_contents || had_multi) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabSelectedAt(old_contents, new_contents,
                                    active_index(), user_gesture));
  }
}
