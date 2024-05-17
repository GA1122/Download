base::string16 TabStrip::GetAccessibleTabName(const Tab* tab) const {
  const int model_index = GetModelIndexOfTab(tab);
  return IsValidModelIndex(model_index) ? controller_->GetAccessibleTabName(tab)
                                        : base::string16();
}
