void SessionService::RestoreSessionFromCommands(
    const std::vector<SessionCommand*>& commands,
    std::vector<SessionWindow*>* valid_windows) {
  std::map<int, SessionTab*> tabs;
  std::map<int, SessionWindow*> windows;

  if (CreateTabsAndWindows(commands, &tabs, &windows)) {
    AddTabsToWindows(&tabs, &windows);
    SortTabsBasedOnVisualOrderAndPrune(&windows, valid_windows);
    UpdateSelectedTabIndex(valid_windows);
  }
  STLDeleteValues(&tabs);
}
