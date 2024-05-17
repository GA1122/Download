bool TabStripModel::CloseTabContentsAt(int index, uint32 close_types) {
  DCHECK(ContainsIndex(index));
  std::vector<int> closing_tabs;
  closing_tabs.push_back(index);
  return InternalCloseTabs(closing_tabs, close_types);
}
