void TabStripModel::ForgetOpenersAndGroupsReferencing(
    const WebContents* tab) {
  for (WebContentsDataVector::const_iterator i = contents_data_.begin();
       i != contents_data_.end(); ++i) {
    if ((*i)->group == tab)
      (*i)->group = NULL;
    if ((*i)->opener == tab)
      (*i)->opener = NULL;
  }
}
