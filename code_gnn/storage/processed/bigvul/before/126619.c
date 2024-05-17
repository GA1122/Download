void TabStripModel::ForgetAllOpeners() {
  for (WebContentsDataVector::const_iterator iter = contents_data_.begin();
       iter != contents_data_.end(); ++iter)
    (*iter)->ForgetOpener();
}
