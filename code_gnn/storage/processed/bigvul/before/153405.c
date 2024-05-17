int TabStrip::GetSizeNeededForTabs(const std::vector<Tab*>& tabs) {
  int width = 0;
  for (const Tab* tab : tabs)
    width += tab->width();
  if (!tabs.empty())
    width -= TabStyle::GetTabOverlap() * (tabs.size() - 1);
  return width;
}
