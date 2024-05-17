bool TabStrip::IsFirstVisibleTab(const Tab* tab) const {
  return GetModelIndexOfTab(tab) == 0;
}
