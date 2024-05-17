bool TabStripGtk::IsTabSelected(const TabGtk* tab) const {
  if (tab->closing())
    return false;

  return model_->IsTabSelected(GetIndexOfTab(tab));
}
