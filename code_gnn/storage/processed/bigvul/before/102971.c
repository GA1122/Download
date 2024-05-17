TabContentsWrapper* TabStripModel::GetSelectedTabContents() const {
  return GetTabContentsAt(active_index());
}
