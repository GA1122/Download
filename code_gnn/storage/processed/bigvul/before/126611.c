void TabStripModel::AddTabAtToSelection(int index) {
  DCHECK(ContainsIndex(index));
  TabStripSelectionModel new_model;
  new_model.Copy(selection_model_);
  new_model.AddIndexToSelection(index);
  SetSelection(new_model, NOTIFY_DEFAULT);
}
