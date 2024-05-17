bool IsCarryOverCandidateForSplitView(
    const MruWindowTracker::WindowList& windows,
    size_t i) {
  return windows.size() > i && CanSnapInSplitview(windows[i]) &&
         !windows[i]->GetProperty(kIsShowingInOverviewKey) &&
         static_cast<ash::AppType>(windows[i]->GetProperty(
             aura::client::kAppType)) != AppType::ARC_APP;
}
