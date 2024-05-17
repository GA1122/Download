TabContents* TabStripModel::GetActiveTabContents() const {
  return GetTabContentsAt(active_index());
}
