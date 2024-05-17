void TabStripModel::ForgetGroup(TabContents* contents) {
  int index = GetIndexOfTabContents(contents);
  DCHECK(ContainsIndex(index));
  contents_data_[index]->SetGroup(NULL);
  contents_data_[index]->ForgetOpener();
}
