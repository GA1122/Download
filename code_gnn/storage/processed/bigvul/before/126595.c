void TabStripGtk::ToggleTabSelection(TabGtk* tab) {
  int index = GetIndexOfTab(tab);
  model_->ToggleSelectionAt(index);
}
