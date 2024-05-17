bool TabStripModel::WillContextMenuPin(int index) {
  std::vector<int> indices = GetIndicesForCommand(index);
  bool all_pinned = true;
  for (size_t i = 0; i < indices.size() && all_pinned; ++i) {
    if (!IsAppTab(index))   
      all_pinned = IsTabPinned(indices[i]);
  }
  return !all_pinned;
}
