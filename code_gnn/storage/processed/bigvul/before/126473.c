void TabStripGtk::ActivateTab(TabGtk* tab) {
  int index = GetIndexOfTab(tab);
  if (model_->ContainsIndex(index))
    model_->ActivateTabAt(index, true);
}
