void TabStripModel::NotifySelectionChanged(int old_selected_index) {
  TabContentsWrapper* old_tab =
      old_selected_index == TabStripSelectionModel::kUnselectedIndex ?
      NULL : GetTabContentsAt(old_selected_index);
  TabContentsWrapper* new_tab =
      active_index() == TabStripSelectionModel::kUnselectedIndex ?
      NULL : GetTabContentsAt(active_index());
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabSelectedAt(old_tab, new_tab, active_index(), true));
}
