TabContents* TabStripModel::GetTabContentsAt(int index) const {
  if (ContainsIndex(index))
    return GetTabContentsAtImpl(index);
  return NULL;
}
