void TabStripModel::ReplaceNavigationControllerAt(
    int index, TabContentsWrapper* contents) {
  InsertTabContentsAt(index + 1, contents, ADD_ACTIVE | ADD_INHERIT_GROUP);
  std::vector<int> closing_tabs;
  closing_tabs.push_back(index);
  InternalCloseTabs(closing_tabs, CLOSE_NONE);
}
