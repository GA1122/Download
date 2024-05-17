TabContents* TabStripModel::ReplaceTabContentsAt(int index,
                                                 TabContents* new_contents) {
  DCHECK(ContainsIndex(index));
  TabContents* old_contents = GetTabContentsAtImpl(index);

  ForgetOpenersAndGroupsReferencing(old_contents->web_contents());

  contents_data_[index]->contents = new_contents->web_contents();

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabReplacedAt(this, old_contents, new_contents, index));

  if (active_index() == index) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      ActiveTabChanged(old_contents, new_contents,
                                       active_index(), false));
  }
  return old_contents;
}
