TabContentsWrapper* TabStripModel::DetachTabContentsAt(int index) {
  if (contents_data_.empty())
    return NULL;

  DCHECK(ContainsIndex(index));

  TabContentsWrapper* removed_contents = GetContentsAt(index);
  int next_selected_index = order_controller_->DetermineNewSelectedIndex(index);
  delete contents_data_.at(index);
  contents_data_.erase(contents_data_.begin() + index);
  ForgetOpenersAndGroupsReferencing(&(removed_contents->controller()));
  if (empty())
    closing_all_ = true;
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
      TabDetachedAt(removed_contents, index));
  if (empty()) {
    FOR_EACH_OBSERVER(TabStripModelObserver, observers_, TabStripEmpty());
  } else {
    int old_active = active_index();
    selection_model_.DecrementFrom(index);
    if (index == old_active) {
      if (!selection_model_.empty()) {
        selection_model_.set_active(selection_model_.selected_indices()[0]);
        selection_model_.set_anchor(selection_model_.active());
        NotifyTabSelectedIfChanged(removed_contents, active_index(), false);
      } else {
        selection_model_.SetSelectedIndex(next_selected_index);
        NotifyTabSelectedIfChanged(removed_contents, next_selected_index,
                                   false);
      }
    }
  }
  return removed_contents;
}
