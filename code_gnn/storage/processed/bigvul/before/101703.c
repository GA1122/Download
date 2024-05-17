void Browser::NavigationStateChanged(const TabContents* source,
                                     unsigned changed_flags) {
  if (changed_flags)
    ScheduleUIUpdate(source, changed_flags);

  if (changed_flags & TabContents::INVALIDATE_URL)
    UpdateCommandsForTabState();
}
