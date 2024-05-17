void TabletModeWindowManager::DisplayConfigurationChanged() {
  RemoveWindowCreationObservers();
  AddWindowCreationObservers();
  UpdateDeskContainersBackdrops();
}
