bool TabStripModel::IsAppTab(int index) const {
  TabContentsWrapper* contents = GetTabContentsAt(index);
  return contents && contents->extension_tab_helper()->is_app();
}
