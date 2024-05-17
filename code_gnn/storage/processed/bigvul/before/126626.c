int TabStripModel::GetIndexOfNextWebContentsOpenedBy(const WebContents* opener,
                                                     int start_index,
                                                     bool use_group) const {
  DCHECK(opener);
  DCHECK(ContainsIndex(start_index));

  for (int i = start_index + 1; i < count(); ++i) {
    if (OpenerMatches(contents_data_[i], opener, use_group))
      return i;
  }
  for (int i = start_index - 1; i >= 0; --i) {
    if (OpenerMatches(contents_data_[i], opener, use_group))
      return i;
  }
  return kNoTab;
}
