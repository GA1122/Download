TabContentsWrapper* TabStripModel::ReplaceTabContentsAt(
    int index,
    TabContentsWrapper* new_contents) {
  DCHECK(ContainsIndex(index));
  TabContentsWrapper* old_contents = GetContentsAt(index);

  ForgetOpenersAndGroupsReferencing(&(old_contents->controller()));

  contents_data_[index]->contents = new_contents;

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabReplacedAt(this, old_contents, new_contents, index));

  if (active_index() == index) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                      TabSelectedAt(old_contents, new_contents, active_index(),
                                    false));
  }
  return old_contents;
}
