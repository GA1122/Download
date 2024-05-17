int TabStripModel::GetIndexOfFirstTabContentsOpenedBy(
    const NavigationController* opener,
    int start_index) const {
  DCHECK(opener);
  DCHECK(ContainsIndex(start_index));

  for (int i = 0; i < start_index; ++i) {
    if (contents_data_[i]->opener == opener)
      return i;
  }
  return kNoTab;
}
