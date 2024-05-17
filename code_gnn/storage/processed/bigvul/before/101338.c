bool SessionModelAssociator::IsValidSessionTab(const SessionTab& tab) {
  if (tab.navigations.empty())
    return false;
  int selected_index = tab.current_navigation_index;
  selected_index = std::max(
      0,
      std::min(selected_index,
          static_cast<int>(tab.navigations.size() - 1)));
  if (selected_index == 0 &&
      tab.navigations.size() == 1 &&
      tab.navigations.at(selected_index).virtual_url().GetOrigin() ==
          GURL(chrome::kChromeUINewTabURL)) {
    return false;
  }
  return true;
}
