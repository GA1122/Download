bool TabStripModel::IsMiniTab(int index) const {
  return IsTabPinned(index) || IsAppTab(index);
}
