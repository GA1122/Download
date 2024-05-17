bool TabStripModel::TabsAreLoading() const {
  for (WebContentsDataVector::const_iterator iter = contents_data_.begin();
       iter != contents_data_.end(); ++iter) {
    if ((*iter)->contents->IsLoading())
      return true;
  }
  return false;
}
