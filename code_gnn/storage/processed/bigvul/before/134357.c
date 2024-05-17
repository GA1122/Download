int TabStrip::GetSizeNeededForTabs(const std::vector<Tab*>& tabs) {
  int width = 0;
  for (size_t i = 0; i < tabs.size(); ++i) {
    Tab* tab = tabs[i];
    width += tab->width();
    if (i > 0 && tab->data().mini != tabs[i - 1]->data().mini)
      width += kMiniToNonMiniGap;
  }
  if (tabs.size() > 0)
    width += tab_h_offset() * static_cast<int>(tabs.size() - 1);
  return width;
}
