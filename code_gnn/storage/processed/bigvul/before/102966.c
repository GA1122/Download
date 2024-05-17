std::vector<int> TabStripModel::GetIndicesClosedByCommand(
    int index,
    ContextMenuCommand id) const {
  DCHECK(ContainsIndex(index));
  DCHECK(id == CommandCloseTabsToRight || id == CommandCloseOtherTabs);
  bool is_selected = IsTabSelected(index);
  int start;
  if (id == CommandCloseTabsToRight) {
    if (is_selected) {
      start = selection_model_.selected_indices()[
          selection_model_.selected_indices().size() - 1] + 1;
    } else {
      start = index + 1;
    }
  } else {
    start = 0;
  }
  std::vector<int> indices;
  for (int i = count() - 1; i >= start; --i) {
    if (i != index && !IsMiniTab(i) && (!is_selected || !IsTabSelected(i)))
      indices.push_back(i);
  }
  return indices;
}
