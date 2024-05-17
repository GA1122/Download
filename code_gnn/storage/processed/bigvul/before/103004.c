void TabStripModel::SetTabPinned(int index, bool pinned) {
  DCHECK(ContainsIndex(index));
  if (contents_data_[index]->pinned == pinned)
    return;

  if (IsAppTab(index)) {
    if (!pinned) {
      NOTREACHED();
      return;
    }
    contents_data_[index]->pinned = pinned;
  } else {
    int non_mini_tab_index = IndexOfFirstNonMiniTab();
    contents_data_[index]->pinned = pinned;
    if (pinned && index != non_mini_tab_index) {
      MoveTabContentsAtImpl(index, non_mini_tab_index, false);
      index = non_mini_tab_index;
    } else if (!pinned && index + 1 != non_mini_tab_index) {
      MoveTabContentsAtImpl(index, non_mini_tab_index - 1, false);
      index = non_mini_tab_index - 1;
    }

    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabMiniStateChanged(contents_data_[index]->contents,
                                          index));
  }

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabPinnedStateChanged(contents_data_[index]->contents,
                                          index));
}
