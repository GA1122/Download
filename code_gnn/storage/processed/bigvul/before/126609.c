void TabStripModel::ActivateTabAt(int index, bool user_gesture) {
  DCHECK(ContainsIndex(index));
  TabStripSelectionModel new_model;
  new_model.Copy(selection_model_);
  new_model.SetSelectedIndex(index);
  SetSelection(new_model, user_gesture ? NOTIFY_USER_GESTURE : NOTIFY_DEFAULT);
}
