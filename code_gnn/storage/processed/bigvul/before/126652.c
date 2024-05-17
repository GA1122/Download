void TabStripModel::SetSelection(
    const TabStripSelectionModel& new_model,
    NotifyTypes notify_types) {
  TabContents* old_contents = GetActiveTabContents();
  TabStripSelectionModel old_model;
  old_model.Copy(selection_model_);
  if (new_model.active() != selection_model_.active())
    NotifyIfTabDeactivated(old_contents);
  selection_model_.Copy(new_model);
  NotifyIfActiveOrSelectionChanged(old_contents, notify_types, old_model);
}
