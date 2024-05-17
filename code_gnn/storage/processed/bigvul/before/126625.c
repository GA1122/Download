int TabStripModel::GetIndexOfLastWebContentsOpenedBy(const WebContents* opener,
                                                     int start_index) const {
  DCHECK(opener);
  DCHECK(ContainsIndex(start_index));

  for (int i = contents_data_.size() - 1; i > start_index; --i) {
    if (contents_data_[i]->opener == opener)
      return i;
  }
  return kNoTab;
}
