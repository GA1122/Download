void TabStripGtk::ExtendTabSelection(TabGtk* tab) {
  int index = GetIndexOfTab(tab);
  if (model_->ContainsIndex(index))
    model_->ExtendSelectionTo(index);
}
