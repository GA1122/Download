void BrowserTabStripController::TabSelectionChanged(
    TabStripModel* tab_strip_model,
    const TabStripSelectionModel& old_model) {
  tabstrip_->SetSelection(old_model, model_->selection_model());
}
