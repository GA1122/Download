void NavigationControllerImpl::LoadEntry(
    std::unique_ptr<NavigationEntryImpl> entry,
    std::unique_ptr<NavigationUIData> navigation_ui_data) {
  DiscardPendingEntry(false);

  SetPendingEntry(std::move(entry));
  NavigateToPendingEntry(ReloadType::NONE, std::move(navigation_ui_data));
}
