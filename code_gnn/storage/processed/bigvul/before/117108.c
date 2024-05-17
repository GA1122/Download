void SessionService::UpdateTabNavigation(
    const SessionID& window_id,
    const SessionID& tab_id,
    int index,
    const NavigationEntry& entry) {
  if (!ShouldTrackEntry(entry.GetVirtualURL()) ||
      !ShouldTrackChangesToWindow(window_id)) {
    return;
  }

  if (tab_to_available_range_.find(tab_id.id()) !=
      tab_to_available_range_.end()) {
    std::pair<int, int>& range = tab_to_available_range_[tab_id.id()];
    range.first = std::min(index, range.first);
    range.second = std::max(index, range.second);
  }
  ScheduleCommand(CreateUpdateTabNavigationCommand(kCommandUpdateTabNavigation,
                                                   tab_id.id(), index, entry));
}
